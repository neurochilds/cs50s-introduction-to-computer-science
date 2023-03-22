import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)
app.secret_key = 'dsjifjif3jfoi3joijf4oi34jfoij34iof'

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        if 'submit-birthday' in request.form:
            try:
                name, month, day = request.form.get("name"), int(request.form.get("month")), int(request.form.get("day"))
            except TypeError:
                flash('Please fill out all fields', 'error')
                return redirect("/")

            if not(name):
                flash('Please fill out all fields', 'error')
                return redirect("/")

            db.execute('INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)', name, month, day)
            flash('Added!', 'success')
            return redirect("/")

        else:
            delete_id = request.form['delete_id']
            db.execute("DELETE FROM birthdays WHERE id = ?", delete_id)
            flash("Deleted!", 'success')
            return redirect("/")

    else:
        birthdays = db.execute('SELECT * FROM birthdays')
        return render_template("index.html",birthdays=birthdays)