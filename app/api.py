from flask import Flask, request, make_response, render_template, url_for

from . import service

api = Flask(__name__)

@api.route('/', methods=['GET'])
def index():
    return render_template('index.html')

@api.route('/capture_imga', methods=['POST'])
def capture_imga():
    msg = service.save_imga(request.form["img"])
    return make_response(msg)

@api.route('/capture_imgb', methods=['POST'])
def capture_imgb():
    msg = service.save_imgb(request.form["img"])
    return make_response(msg)

