import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    shares = db.execute("SELECT symbol, name, SUM(shares) as shares FROM orders WHERE user_id = :user_id GROUP BY symbol",
                        user_id=session['user_id'])

    rows = db.execute("SELECT cash FROM users WHERE id=:user_id", user_id=session["user_id"])
    own_cash = rows[0]['cash']

    html = ""
    total = 0.0
    for share in shares:
        if int(share['shares']) <= 0:
            continue
        res = lookup(share['symbol'])
        current_val = float(res['price']) * float(share['shares'])
        total += current_val
        html += "<tr>"
        html += "<td>" + share['symbol'] + "</td>"
        html += "<td>" + share['name'] + "</td>"
        html += "<td>" + str(share['shares']) + "</td>"
        html += "<td>$" + "{0:.2f}".format(res['price']) + "</td>"
        html += "<td>$" + "{0:.2f}".format(current_val) + "</td>"
        html += "</tr>"

    return render_template("index.html", html=html, own_cash=float(own_cash), total=float(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # Require that a user input a stock’s symbol, implemented as a text field.
    # Render an apology if the input is blank or the symbol does not exist (as per the return value of lookup
    if request.method == "POST":
        res = lookup(request.form.get("symbol"))
        if res == None or res['symbol'] == "":
            return apology("Invalid symbol")
        curr_price = res['price']
        if not request.form.get("shares").isdigit():
            return apology("Invalid amount of shares!")
        shares = int(request.form.get("shares", 0))
        if shares < 1:
            return apology("Shares must have a positive value")
        if curr_price == "":
            return apology("Invalid symbol")

        # Query database for user's cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session['user_id'])
        cash = rows[0]["cash"]
        remaining_cash = cash - (curr_price * shares)
        if remaining_cash < 0:
            return apology("Insufficient funds!")

        # Creating order
        db.execute("INSERT INTO orders (user_id, symbol, name, price, shares) VALUES (:user_id, :symbol, :name, :price, :shares)",
                   user_id=session['user_id'],
                   symbol=res['symbol'].upper(),
                   name=res['name'],
                   price=res['price'],
                   shares=request.form.get("shares"))

        # Updating user cash
        db.execute("UPDATE users SET cash=:cash WHERE id=:user_id",
                   user_id=session["user_id"],
                   cash=remaining_cash)

        # Redirect home
        return redirect('/')

    return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    # Query database for username
    username = request.args.get("username", "")
    rows = db.execute("SELECT * FROM users WHERE username = :username",
                      username=username)

    # Ensure username doesn't exists
    if len(rows) != 0 or len(username) < 1:
        return jsonify(False)

    return jsonify(True)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    shares = db.execute("SELECT * FROM orders WHERE user_id = :user_id", user_id=session['user_id'])

    rows = db.execute("SELECT cash FROM users WHERE id=:user_id", user_id=session["user_id"])
    own_cash = rows[0]['cash']

    html = ""
    total = 0.0
    for share in shares:
        if int(share['shares']) < 0:
            color = "danger"
            order = "sold"
        else:
            color = "success"
            order = "bought"

        html += "<tr class=\"text-" + color + "\">"
        html += "<td>" + order + "</td>"
        html += "<td>" + share['symbol'] + "</td>"
        html += "<td>" + share['name'] + "</td>"
        html += "<td>" + str(share['shares']) + "</td>"
        html += "<td>$" + "{0:.2f}".format(share['price']) + "</td>"
        html += "<td>" + share['timestamp'] + "</td>"
        html += "</tr>"

    return render_template("history.html", html=html)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
        res = lookup(request.form.get("symbol"))
        if res == None or res['symbol'] == "":
            return apology("Symbol not found")
        return render_template("quoted.html", result=res)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        # Ensure passowrd matches
        elif not request.form.get("confirmation") or request.form.get("confirmation") != request.form.get("password"):
            return apology("password and confirmation password must be the same", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username doesn't exists
        if len(rows) != 0:
            return apology("Username already taken", 400)

        # Query database for username
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :h)",
                   username=request.form.get("username"),
                   h=generate_password_hash(request.form.get("password")))

        # Redirect user to login page
        return redirect("/login")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol", "")
        shares = int(request.form.get("shares", 0))
        if symbol == "":
            return apology("Symbol is required")

        share_rows = db.execute("SELECT symbol, SUM(shares) as shares FROM orders WHERE user_id = :user_id  AND symbol = :symbol GROUP BY symbol",
                                user_id=session['user_id'], symbol=symbol)

        if shares < 1 or len(share_rows) == 0 or share_rows[0]['shares'] < shares:
            return apology("Cannot sell more than you own")

        # Fetch prices
        res = lookup(symbol)
        if res == None or res['symbol'] == "":
            return apology("Invalid symbol")
        curr_price = res['price']

        # Query database for user's cash
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session['user_id'])
        cash = rows[0]["cash"]
        remaining_cash = cash + (curr_price * shares)

        # Creating order
        db.execute("INSERT INTO orders (user_id, symbol, name, price, shares) VALUES (:user_id, :symbol, :name, :price, :shares)",
                   user_id=session['user_id'],
                   symbol=res['symbol'].upper(),
                   name=res['name'],
                   price=res['price'],
                   shares=(shares * -1))

        # Updating user cash
        db.execute("UPDATE users SET cash=:cash WHERE id=:user_id",
                   user_id=session["user_id"],
                   cash=remaining_cash)

        return redirect("/")

    # Get all shares
    shares = db.execute("SELECT symbol, SUM(shares) as shares FROM orders WHERE user_id = :user_id  GROUP BY symbol",
                        user_id=session['user_id'])

    # Build stock options
    html = ""
    for share in shares:
        html += "<option value=\"" + share["symbol"] + "\">"
        html += share["symbol"] + " (" + str(share['shares']) + ")"
        html += "</option>"

    return render_template("sell.html", html=html)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
