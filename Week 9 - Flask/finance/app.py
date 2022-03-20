import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime, timezone
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # return apology("TODO")

    user = session["user_id"]
    total = 0

    # Summarize user's stock ownership
    owns = db.execute("SELECT symbol, SUM(shares) as shares FROM purchases GROUP BY symbol HAVING user_id = ? AND SUM(shares) > 0", user)

    # Get user's cash balance
    balance = db.execute("SELECT cash FROM users WHERE id = ?", user)[0]['cash']

    # Get table elements
    for own in owns:
        result = lookup(own["symbol"])
        own["name"] = result["name"]
        own["price"] = result["price"]
        own["value"] = own["price"] * own["shares"]
        total += own["value"]
    total += balance

    # Display table
    return render_template("index.html", owns=owns, balance=usd(balance), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # return apology("TODO")

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        symbol = request.form.get("symbol")
        result = lookup(symbol)

        # Ensure symbol exists
        if not result:
            return apology("invalid symbol", 400)

        # Ensure number of shares was submitted
        elif not request.form.get("shares"):
            return apology("must provide number of shares", 400)

        elif request.form.get("shares").isdigit() == False:
            return apology("invalid number of shares", 400)

        shares = int(request.form.get("shares"))

        # Ensure number of shares submitted is a positive integer
        if shares < 1:
            return apology("invalid number of shares", 400)

        user = session["user_id"]
        balance = db.execute("SELECT cash FROM users WHERE id = ?", user)[0]['cash']
        price = result["price"]
        total = price * shares

        # Ensure cash balance is sufficient
        if balance < total:
            return apology("insufficient cash balance", 400)

        # Update purchase log
        db.execute("INSERT INTO purchases (user_id, symbol, shares, price, time) VALUES(?, ?, ?, ?, ?)",
                   user, symbol, shares, total, datetime.now())

        # Update cash balance
        db.execute("UPDATE users SET cash = ? WHERE id = ?", (balance - total), user)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # return apology("TODO")

    user = session["user_id"]

    # Summarize user's stock ownership
    owns = db.execute("SELECT symbol, shares, price, time FROM purchases symbol WHERE user_id = ?", user)

    # Display table
    return render_template("history.html", owns=owns)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # return apology("TODO")

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        result = lookup(request.form.get("symbol"))

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure symbol exists
        elif not result:
            return apology("invalid symbol", 400)

        # Return stock's current price
        return render_template("quoted.html", name=result["name"], price=usd(result["price"]), symbol=result["symbol"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # return apology("TODO")

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure username is unique
        elif len(db.execute('SELECT username FROM users WHERE username = ?', request.form.get("username"))) != 0:
            return apology("username taken", 400)

        elif len(request.form.get("password")) < 8:
            return apology("password must have at least 8 characters", 400)

        # Ensure password matches confirmation
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password does not match confirmation", 400)

        # Add username to database
        db.execute("INSERT INTO users (username, hash) VALUES(?,?)", request.form.get("username"),
                   generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # return apology("TODO")

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        user = session["user_id"]
        symbol = request.form.get("symbol")
        result = db.execute("SELECT symbol FROM purchases GROUP BY symbol HAVING user_id = ? AND SUM(shares) > 0", user)

        # Ensure user owns symbol
        if not result:
            return apology("invalid symbol", 400)

        # Ensure number of shares was submitted
        elif not request.form.get("shares"):
            return apology("must provide number of shares", 400)

        shares = int(request.form.get("shares"))
        owns = db.execute(
            "SELECT SUM(shares) as shares FROM purchases GROUP BY symbol HAVING user_id = ? AND SUM(shares) > 0 AND symbol = ?", user, symbol)[0]['shares']

        # Ensure number of shares to sell is enough
        if shares > owns:
            return apology("insufficient number of shares", 400)

        balance = db.execute("SELECT cash FROM users WHERE id = ?", user)[0]['cash']
        price = lookup(symbol)["price"]
        total = price * shares

        # Update purchase log
        db.execute("INSERT INTO purchases (user_id, symbol, shares, price, time) VALUES(?, ?, ?, ?, ?)",
                   user, symbol, -(shares), total, datetime.now())

        # Update cash balance
        db.execute("UPDATE users SET cash = ? WHERE id = ?", (balance + total), user)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        user = session["user_id"]
        owns = db.execute("SELECT symbol FROM purchases GROUP BY symbol HAVING user_id = ? AND SUM(shares) > 0", user)
        return render_template("sell.html", owns=owns)
