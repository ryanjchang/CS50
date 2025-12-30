import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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
    #for each share owned, display all details Name, symbol, price, owned, value(price * owned)
    # #use lookup, make a new dict shares and pass in
    shares = []
    user_shares = db.execute("SELECT * FROM shares WHERE user_id = ?", session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id= ?", session["user_id"])

    for row in user_shares:
        symbol = row["symbol"]
        stock = lookup(symbol)

        shares.append({
            "name" : stock["name"],
            "symbol" : symbol,
            "price" : usd(stock["price"]),
            "owned" : row["owned"],
            "value" : usd(stock["price"] * row["owned"]),
        })

    return render_template("index.html", cash=usd(cash[0]["cash"]), shares=shares)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol required")
        shares = request.form.get("shares")
        if not shares:
            return apology("Number of shares required")
        try:
            shares = int(shares)
            if shares < 1:
                return apology("Shares must be a positive integer")
        except ValueError:
            return apology("Shares must be a number")

        stock = lookup(symbol) #stores name, price, and symbol

        user_id = session["user_id"]
        owned_stock = db.execute("SELECT owned FROM shares WHERE user_id = ? AND symbol = ?", user_id, symbol)
        if owned_stock:
            db.execute("UPDATE shares SET owned = ? WHERE user_id = ? AND symbol = ?",
                owned_stock[0]["owned"] + shares, user_id, symbol)
        else:
            db.execute("INSERT INTO shares(user_id, symbol, owned) VALUES(?,?,?)",
                user_id, symbol, shares)
        #shares
        #id (primary key) INT
        #user_id INT, not null
        #symbol text, not null
        #owned INT NOT NULL, DEFAULT 0

        current_cash = db.execute("SELECT cash FROM USERS WHERE id = ?", user_id)
        cash_required = stock["price"] * shares
        if current_cash[0]["cash"] < cash_required:
            return apology("Insufficient funds")
        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                       current_cash[0]["cash"] - cash_required, user_id)

            db.execute("INSERT INTO transactions (user_id, symbol, action, price, count) VALUES (?, ?, ?, ?, ?)",
                       user_id, symbol, 'buy', stock["price"], shares)


        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    #transactions
    #id int primary key
    #user_id int not null
    #symbol text not null
    #action text constraint check in (buy, sell)
    #price number not null
    #count int not null
    #order_date DATE DEFAULT CURRENT_DATE

    transactions = []

    #need to update buy/sell functions to record each transaction
    #list of transactions where user id = session user id
    past_actions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    for action in past_actions:
        stock = lookup(action["symbol"])
        transactions.append({
            "name" : stock["name"],
            "symbol" : action["symbol"],
            "price" : action["price"],
            "count" : action["count"],
            "value" : action["price"] * action["count"],
            "action" : action["action"],
            "date" : action["order_date"]
        })

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

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
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol required")

        stock = lookup(symbol) #stores name, price, and symbol
        return render_template("quoted.html", name=stock["name"], price=stock["price"], symbol=stock["symbol"])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        if not username:
            return apology("Username required")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not password or not confirmation:
            return apology("Need password and confirm password")
        if password != confirmation:
            return apology("password must match confirmation")


        try:
            hash = generate_password_hash(password, method='scrypt', salt_length=16)
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)
        except ValueError:
            return apology("Username already exists")

        rows = db.execute("SELECT * FROM users WHERE username=?", username)
        session["user_id"] = rows[0]["id"]
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol required")
        shares = request.form.get("shares")
        owned_shares = db.execute("SELECT owned FROM shares WHERE user_id = ? AND symbol = ?",
                                  session["user_id"], symbol)
        if not shares:
            return apology("Number of shares required")
        if not owned_shares:
            return apology("No shares owned")
        try:
            shares = int(shares)
            if shares < 1:
                return apology("Shares must be a positive integer")
            if owned_shares[0]["owned"] < shares:
                return apology("Can not sell more shares than owned")
        except ValueError:
            return apology("Shares must be a number")

        db.execute("UPDATE shares SET owned = ? WHERE user_id = ? AND symbol = ?",
                   owned_shares[0]["owned"] - shares, session["user_id"], symbol)
        stock = lookup(symbol)
        if not stock:
            return apology("Company does not exist. Can not sell")
        db.execute("INSERT INTO transactions (user_id, symbol, action, price, count) VALUES (?, ?, ?, ?, ?)",
                       session["user_id"], symbol, 'sell', stock["price"], shares)

        return redirect("/")
    else:
        return render_template("sell.html")

@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    if request.method == "POST":
        previous_password = request.form.get("previous_password")
        if not previous_password:
            return apology("must provide previous password")
        new_password = request.form.get("new_password")
        if not new_password:
            return apology("must provide new password")
        #verify previous password matches
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], previous_password):
            return apology("incorrect previous password")
        #hash new password and update hash
        hash = generate_password_hash(new_password, method='scrypt', salt_length=16)
        db.execute("UPDATE users SET hash=? WHERE id=?", hash, session["user_id"])

        return redirect("/")
    else:
        return render_template("settings.html")
