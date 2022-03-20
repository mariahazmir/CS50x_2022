-- Keep a log of any SQL queries you execute as you solve the mystery.

/*  Authorities: "The thief stole the duck and then, shortly afterwards, took a flight out of town with the help of an accomplice."
    Public Info:
    Date    : July 28, 2021
    Location: Humphrey Street
*/


-- 1. Look into crime scene reports logged on July 28, 2021, on Humphrey Street

SELECT description
FROM crime_scene_reports
WHERE day = 28
AND month = 7
AND year = 2021
AND street = "Humphrey Street";

/*  Information acquired:
    Time: 10:15am
    Location: Humphrey Street bakery
    "Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery."
    (Ignore littering log)
*/


-- 2. Get interview transcripts that mention Humphrey Street bakery (3 in total)

SELECT name, transcript
FROM interviews
WHERE day = 28
AND month = 7
AND year = 2021
AND transcript LIKE '%bakery%';

/*  Information acquired:
    Witnesses:
    1. Ruth   : Thief got out of bakery parking lot in a car within 10 minutes of theft. (3)
    2. Eugene : Thief withdrawed money from ATM on Leggett Street before theft. (4)
    3. Raymond: Thief planned to take earliest flight the next day. (5)
    4. Emma(redundant)
*/


-- 3. Identify cars that left the bakery 10 minutes after the theft

SELECT license_plate, activity
FROM bakery_security_logs
WHERE day = 28
AND month = 7
AND year = 2021
AND hour = 10
AND minute <= 25
AND minute >= 15;

/*  Information acquired:
    License plates of cars that left the bakery:
    5P2BI95
    94KL13X
    6P58WS2
    4328GD8
    G412CB7
    L93JTIZ
    322W7JE
    0NTHK55
*/


-- 4. Get ATM of Leggett Street's logs before the theft

SELECT account_number, amount
FROM atm_transactions
WHERE day = 28
AND month = 7
AND year = 2021
AND atm_location = "Leggett Street"
AND transaction_type = "withdraw";

/*  Information acquired:
    | Account number | Amount |
    | 28500762       | 48     |
    | 28296815       | 20     |
    | 76054385       | 60     |
    | 49610011       | 50     |
    | 16153065       | 80     |
    | 25506511       | 20     |
    | 81061156       | 30     |
    | 26013199       | 35     |
*/


-- 5. Identify first flight on July 29, 2021

SELECT abbreviation, full_name, city, flights.hour, flights. minute, flights.id
FROM airports
JOIN flights ON airports.id = destination_airport_id
WHERE(
    SELECT destination_airport_id
    FROM flights
    WHERE (
        SELECT id
        FROM airports
        WHERE city = "Fiftyville"
    ) = origin_airport_id
    AND day = 29
    AND month = 7
    AND year = 2021
    ORDER BY hour ASC, minute ASC
    LIMIT 1
) = airports.id
ORDER BY hour ASC, minute ASC
LIMIT 1;

/*  Information acquired:
    | abbreviation |     full_name     |     city      | hour | minute | id |
    | LGA          | LaGuardia Airport | New York City | 8    | 20     | 36 |
*/

-- ANSWER 1: THIEF ESCAPED TO NEW YORK CITY


-- 6. Match car license plate with ATM account number and flight information

SELECT name
FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE people.license_plate = "5P2BI95"
    OR people.license_plate = "94KL13X"
    OR people.license_plate = "6P58WS2"
    OR people.license_plate = "4328GD8"
    OR people.license_plate = "G412CB7"
    OR people.license_plate = "L93JTIZ"
    OR people.license_plate = "322W7JE"
    OR people.license_plate = "0NTHK55"
AND bank_accounts.account_number = 28500762
    OR bank_accounts.account_number = 28296815
    OR bank_accounts.account_number = 76054385
    OR bank_accounts.account_number = 49610011
    OR bank_accounts.account_number = 16153065
    OR bank_accounts.account_number = 25506511
    OR bank_accounts.account_number = 81061156
    OR bank_accounts.account_number = 26013199
INTERSECT
SELECT name
FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
WHERE flights.id = 36;

/*  Acquired information:
    Remaining suspects:
    1. Bruce
    2. Kenny
    3. Luca
    4. Taylor
*/


-- 7. Get suspects' phone numbers

SELECT name, phone_number
FROM people
WHERE name = "Bruce"
OR name = "Kenny"
OR name = "Luca"
OR name = "Taylor";

/*  Information acquired:
    |  name  |  phone_number  |
    | Kenny  | (826) 555-1652 |
    | Taylor | (286) 555-6063 |
    | Luca   | (389) 555-5198 |
    | Bruce  | (367) 555-5533 |
*/


-- 8. Match phone number with call logs

SELECT people.name, caller, receiver, duration, day
FROM phone_calls
JOIN people ON phone_calls.caller = people.phone_number
wHERE name = "Kenny"
OR name = "Bruce"
OR name = "Luca"
OR name = "Taylor"
AND duration < 60
AND month = 7
AND year = 2021
AND day = 28;

/*  Information acquired:
    |  name  |     caller     |    receiver    |
    | Bruce  | (367) 555-5533 | (375) 555-8161 |
    | Taylor | (286) 555-6063 | (676) 555-6554 |
    | Kenny  | (826) 555-1652 | (066) 555-9701 |

    Remaining suspects:
    1. Bruce
    2. Kenny
    3. Taylor
*/


-- 9. Get receiver names

SELECT name, phone_number
FROM people
WHERE phone_number = "(375) 555-8161"
OR phone_number = "(676) 555-6554"
OR phone_number = "(066) 555-9701";

/*  Information acquired:
    Remaining suspects' accomplice
    1. James
    2. Robin
    3. Doris
*/


-- Bruce called Robin for less than a minute
-- The thief is Bruce
-- The accomplice is Robin