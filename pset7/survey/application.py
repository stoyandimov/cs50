import cs50
import csv
import os.path

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    # Default
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    # Redirect to form page
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    # Show survey form
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    # Validate user input
    err_msg = ""
    name = request.form.get("name", "")
    if name is "":
        err_msg += "<li>Your name is required!</li>"
    gender = request.form.get("gender", "")
    if gender is "":
        err_msg += "<li>Your gender is required!</li>"
    age = request.form.get("age", "")
    if age is "":
        err_msg += "<li>Your age is required!</li>"
    school_year = request.form.get("school_year", "")
    if school_year is "":
        err_msg += "<li>Your school year is required!</li>"

    if err_msg is not "":
        msg = "<ul>" + err_msg + "</ul>"
        return render_template("error.html", message=msg)

    # Else add row to the survey.csv
    file_exists = os.path.isfile('survey.csv')
    with open('survey.csv', 'a', newline='') as csvfile:
        fieldnames = ['name', 'gender', 'age', 'school_year']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        if not file_exists:
            writer.writeheader()
        writer.writerow({'name': name, "gender": gender, "age": age, "school_year": school_year})

    return redirect("/sheet")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    # Build the HTML table rows from csv file.
    with open('survey.csv', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        html = ""
        for row in reader:
            html += "<tr>"
            html += "    <td>" + row['name'] + "</td>"
            html += "    <td>" + row['gender'] + "</td>"
            html += "    <td>" + row['age'] + "</td>"
            html += "    <td>" + row['school_year'] + "</td>"
            html += "</tr>"

    return render_template("sheet.html", html_rows=html)
