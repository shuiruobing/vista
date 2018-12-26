# -*- coding: utf-8 -*-
# Author: thansky


from flask import request, Response, jsonify, Blueprint, g, render_template
from flask import url_for, redirect
from flask_login import LoginManager, login_user, current_user, login_required
from ..models.users import UserStorage


user_view = Blueprint('users', __name__, url_prefix='/api/v1/user')
login_mgr = LoginManager()
login_mgr.login_view = "gennerals.grl_main"
login_mgr.login_message = u"Bonvolu ensaluti por uzi tiun paĝon."
login_mgr.login_message_category = "info"
login_mgr.session_protection = "strong"


@login_mgr.user_loader
def load_user(user_token):
    return UserStorage.check_on_line(user_token)


@login_mgr.unauthorized_handler
def unauthorized():
    return redirect(url_for('gennerals.grl_login'))


@user_view.route('/Login', methods=['GET', 'POST'])
def api_login():
    username = request.args.get('username')
    password = request.args.get('password')
    user = UserStorage.authenticate(username, password)
    if user is None:
        # 这里还有问题，需要修复
        return jsonify({"Result": -100, "Message": "Authorize failed!"})

    login_user(user)
    return jsonify({"Result": 0, "Access": {
                    "Token": user.token, "Expires": 30}})


@user_view.route('/Logout', methods=['GET'])
@login_required
def api_logout():
    UserStorage.off_line()


@user_view.route('/Keepalive', methods=['GET'])
@login_required
def api_keepalive():
    pass


@user_view.route('/TokenExpire', methods=['GET'])
@login_required
def api_token_expire():
    pass


@user_view.route('/MachineCode', methods=['GET'])
@login_required
def api_machine_code():
    pass


@user_view.route('/Regist', methods=['GET'])
@login_required
def api_regist():
    pass
