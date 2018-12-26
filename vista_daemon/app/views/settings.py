# -*- coding: utf-8 -*-
# Author: thansky


from flask import request, Response, jsonify, Blueprint, render_template


settings_view = Blueprint('settings', __name__, url_prefix='/api/v1/setting')


@settings_view.route('/NewPassword')
def set_new_pwd():
    pass
