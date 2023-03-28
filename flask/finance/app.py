import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

import string

# Configure application
app = Flask(__name__)

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
    user_id = session["user_id"]
    balances = db.execute('SELECT symbol, shares FROM balances WHERE user_id = ?;', user_id)
    cash_balance = db.execute('SELECT cash FROM users WHERE id = ?;', user_id)[0]['cash']

    balances_list = []
    invested_balance = 0
    for balance in balances:
        balance['price'] = lookup(balance['symbol'])['price']
        balance['name'] = lookup(balance['symbol'])['name']
        balances_list.append(balance)

        invested_balance += (balance['price'] * balance['shares'])

    return render_template('index.html', balances=balances_list, cash_balance=cash_balance, invested_balance=invested_balance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        user_id = session["user_id"]

        if not symbol:
            return apology("must provide symbol", 400)
        if not lookup(symbol):
            return apology("symbol does not exist", 400)
        try:
            shares = int(shares)
        except:
            return apology("shares must be a positive integer", 400)
        if shares < 1:
            return apology("shares must be a positive integer", 400)

        symbol = symbol.upper()
        symbol_info = lookup(symbol)
        cost = shares * symbol_info['price']
        user = db.execute('SELECT * FROM users WHERE id = ?', user_id)
        cash = user[0]['cash']
        if cost > cash:
            return apology("you can't afford these shares!", 400)

        db.execute('UPDATE users SET cash = ? WHERE id = ?', (cash - cost), user_id)
        db.execute('INSERT INTO transactions (user_id, transaction_type, symbol, shares, price, total_value) VALUES (?, ?, ?, ?, ?, ?)', user_id, 'PURCHASE', symbol_info['symbol'], shares, symbol_info['price'], cost)
        db.execute('''
            INSERT OR REPLACE INTO balances (id, user_id, symbol, shares)
            VALUES (
                (SELECT id FROM balances WHERE user_id = ? AND symbol = ?),
                ?,
                ?,
                (SELECT IFNULL((SELECT shares FROM balances WHERE user_id = ? AND symbol = ?), 0) + ?));''',
                user_id, symbol, user_id, symbol, user_id, symbol, shares)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions = db.execute('SELECT timestamp, transaction_type, symbol, shares, price, total_value FROM transactions WHERE user_id = ?', user_id)
    deposits_withdrawals =  db.execute('SELECT timestamp, transaction_type, transaction_amount FROM deposits_withdrawals WHERE user_id = ?', user_id)
    return render_template('history.html', transactions=transactions, deposits_withdrawals=deposits_withdrawals)


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
    if request.method == "POST":
        symbol = lookup(request.form.get("symbol"))
        if not symbol:
            return apology("invalid stock symbol")
        return render_template("quoted.html", symbol=symbol)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        password_hash = generate_password_hash(password)
        usernames = db.execute("SELECT username FROM users;")

        if not (username):
            return apology("must provide username", 400)

        for user in usernames:
            if user['username'] == username:
                return apology("username already exists", 400)

        if not (password or confirmation):
            return apology("must provide password and confirmation password", 400)

        contains_letter = any(ch.isalpha() for ch in password)
        contains_number = any(ch.isdigit() for ch in password)
        contains_special = any(ch in string.punctuation for ch in password)

        if not (contains_letter and contains_number and contains_special):
            return apology("password must be at least 8 characters and contain at least one letter, number and special character", 400)

        if username == password:
            return apology("username and password must not match", 400)

        if not (password == confirmation):
             return apology("password and confirmation password do not match", 400)

        db.execute("INSERT into users (username,  hash) VALUES (?, ?)", username, password_hash)
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol", 400)
        if not lookup(symbol):
            return apology("symbol does not exist", 400)
        try:
            shares = int(shares)
        except:
            return apology("shares must be a positive integer", 400)
        if shares < 1:
            return apology("shares must be a positive integer", 400)

        symbol = symbol.upper()
        current_shares = db.execute('SELECT shares FROM balances WHERE user_id = ? AND symbol = ?', user_id, symbol)
        if current_shares[0]['shares'] < shares:
            return apology(f'you own less than {shares} shares', 400)

        symbol_info = lookup(symbol)
        value = shares * symbol_info['price']

        db.execute('UPDATE users SET cash = cash + ? WHERE id = ?', value, user_id)
        db.execute('INSERT INTO transactions (user_id, transaction_type, symbol, shares, price, total_value) VALUES (?, ?, ?, ?, ?, ?)', user_id, 'SALE', symbol_info['symbol'], shares, symbol_info['price'], value)

        if current_shares[0]['shares'] == shares:
            db.execute('DELETE FROM balances WHERE user_id = ? AND symbol = ?', user_id, symbol)

        else:
            db.execute('UPDATE balances SET shares = shares - ? WHERE user_id = ? AND symbol = ?', shares, user_id, symbol)
        return redirect("/")

    else:
        stocks_owned = db.execute('SELECT symbol FROM balances WHERE user_id = ?;', user_id)
        return render_template("sell.html", stocks_owned=stocks_owned)

@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit cash"""
    if request.method == "POST":
        user_id = session["user_id"]
        amount = request.form.get("deposit")

        if not amount:
            return apology("must enter a deposit amount", 400)

        try:
            amount = int(amount)
        except:
            return apology("amount must be a positive integer", 400)

        if amount < 1:
            return apology("amount must be a positive integer", 400)

        cash_balance = db.execute('SELECT cash FROM users WHERE id = ?;', user_id)[0]['cash']
        if amount + cash_balance > 1000000000:
              return apology("cash balance cannot exceed 1,000,000,000", 400)

        db.execute('UPDATE users SET cash = cash + ? WHERE id = ?', amount, user_id)
        db.execute('INSERT INTO deposits_withdrawals (user_id, transaction_type, transaction_amount) VALUES (?, ?, ?)', user_id, 'DEPOSIT', amount)

        return redirect("/")

    else:
        return render_template("deposit.html")

@app.route("/withdraw", methods=["GET", "POST"])
@login_required
def withdraw():
    """Deposit cash"""
    if request.method == "POST":
        user_id = session["user_id"]
        amount = request.form.get("withdraw")

        if not amount:
            return apology("must enter a withdrawal amount", 400)

        try:
            amount = int(amount)
        except:
            return apology("amount must be a positive integer", 400)

        if amount < 1:
            return apology("amount must be a positive integer", 400)

        cash_balance = db.execute('SELECT cash FROM users WHERE id = ?;', user_id)[0]['cash']
        if cash_balance - amount < 0:
              return apology("not enough money in your account", 400)

        db.execute('UPDATE users SET cash = cash - ? WHERE id = ?', amount, user_id)
        db.execute('INSERT INTO deposits_withdrawals (user_id, transaction_type, transaction_amount) VALUES (?, ?, ?)', user_id, 'WITHDRAWAL', amount)

        return redirect("/")

    else:
        return render_template("withdraw.html")