import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Ensure environment variable is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

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

    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    transactionlist = db.execute("SELECT * FROM transactions WHERE id = :id", id=session["user_id"])
    sum = cash[0]["cash"]

    for tra in transactionlist:
        symbol = str(tra["symbol"])
        shares = tra["shares"]
        name = str(tra["name"])
        price = (lookup(request.form.get("symbol")))["price"]
        cost = tra["cost"]
        sum += cost
        db.execute("UPDATE transactions SET price=:price, \
                    total=:total WHERE id=:id AND symbol=:symbol", \
                    price=price, \
                    cost=usd(cost), id=session["user_id"], symbol=symbol)

    sum = "{:.2f}".format(sum)

    return render_template("index.html", transactionlist=transactionlist, sum = sum, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        symbol=lookup(request.form.get("symbol"))
        shares=(request.form.get("shares"))

        if not shares or not symbol:
            return apology("please provide ticker and shares", 400)

        if not shares.isdigit():
            return apology("provide valid number of shares")

        if shares <= 0:
            return apology("provide a valid number of shares", 400)

        cost = entry["price"] * shares
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])

        if cost > cash[0]["cash"]:
            return apology ("not enough money", 400)
        else:
            db.execute("INSERT INTO transactions (name, id, price, symbol, cost, shares) VALUES (:name, :id, :price, :symbol, :cost, :shares)", name=entry["name"], id = session["user_id"], price=usd(entry["price"]), symbol=request.form.get("symbol"), cost=usd(cost), shares=shares)
            db.execute("UPDATE users SET cash = cash - :cost WHERE id = :id", cost = usd(cost), id = session["user_id"])

        return render_template("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    h = db.execute("SELECT symbol, shares, price, date FROM history WHERE id = :id ORDER BY date DESC", id = session["user_id"])

    for t in h:
        date = t["date"]
        symbol = t["symbol"]
        shares = t["shares"]
        price = t["price"]

    return render_template("history.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

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
        return render_template("index.html")

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
        if not request.form.get("symbol"):
            return apology("Please enter a symbol", 400)


        quote=lookup(request.form.get("symbol"))

        if quote == None:
            return apology("not a symbol", 400)

        return render_template("quoted.html", symbol=quote["symbol"], name=quote["name"], price=usd(quote["price"]))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "GET":
        return render_template("register.html")
    else:
        if not request.form.get("username"):
            return apology("provide username", 400)

        elif not request.form.get("password"):
            return apology("provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("provide confirmation", 400)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords do not match", 400)

        phash = generate_password_hash(request.form.get("password"))

        entry = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username = request.form.get("username"), hash = phash)

        if not entry:
            return apology("username taken", 400)

        usersession = db.execute("SELECT * FROM users WHERE username = :username", username = request.form.get("username"))
        session["user_id"] = usersession[0]["id"]

        # Redirect user to home page
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        if not request.form.get("shares") or not request.form.get("symbol"):
            return apology("provide symbol and shares")

        entry = lookup(request.form.get("symbol"))

        if entry == None:
            return apology("not a ticker")

        if not request.form.get("shares").isdigit():
            return apology("not a number of shares")

        transactionlist = db.execute("SELECT * FROM transactions WHERE id = :id AND symbol = :symbol", id=session["user_id"], symbol=request.form.get("symbol"))

        if transactionlist == []:
            return apology("do not own any of the shares")
        else:
            symbol = request.form.get("symbol")
            shares = request.form.get("shares")
            name = request.form.get("symbol")
            price = (lookup(request.form.get("symbol")))["price"]
            cost = -(price*shares)

            db.execute("INSERT INTO transactions (name, id, price, symbol, cost, shares) VALUES (:name, :id, :price, :symbol, :cost, :shares)", name=entry["name"], id = session["user_id"], price=usd(entry["price"]), symbol=request.form.get("symbol"), cost=cost, shares=shares)
            db.execute("UPDATE users SET cash = cash + :cost WHERE id = :id", cost = cost, id = session["user_id"])

            return redirect("/")

    else:
        return render_template("sell.html")
#-------------------------------------------------------------------------------------------------------

def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
