# -*- coding: utf-8 -*-
# Author: thansky


from flask import request, jsonify, Blueprint, render_template
from flask import url_for, redirect
import flask_login


genneral_view = Blueprint('gennerals', __name__,)


@genneral_view.route('/login', methods=['GET', 'POST'])
@genneral_view.route('/login.html', methods=['GET', 'POST'])
def grl_login():
    a = render_template('login.html')
    return a


@genneral_view.route('/', methods=['GET', 'POST'])
@genneral_view.route('/main', methods=['GET', 'POST'])
@genneral_view.route('/main.html', methods=['GET', 'POST'])
@flask_login.login_required
def grl_main():
    # token = request.args.get("Token")
    # if(token is not None):
    return render_template('main.html')
    # return redirect(url_for('gennerals.grl_login'))


@genneral_view.route('/form.html', methods=['GET', 'POST'])
@flask_login.login_required
def grl_form():
    return render_template('form.html')
