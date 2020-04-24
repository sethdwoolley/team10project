from flask import Flask
from user_data import user_api
from group_data import group_api
from device_data import device_api
from backendapi import backend_api
import logging

app = Flask(__name__)
app.debug = True

app.register_blueprint(user_api)
app.register_blueprint(group_api)
app.register_blueprint(device_api)
app.register_blueprint(backend_api)


@app.route('/', methods=["GET"])
def home():
    return "<h1>API Testing</h1></br><h2><i>don't</i> expect <b>ANY</b> cool content on this page!</h2>"


if __name__ == "__main__":
    app.run()
