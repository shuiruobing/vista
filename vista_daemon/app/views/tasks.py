# -*- coding: utf-8 -*-
# Author: thansky


from flask import request, jsonify, Blueprint


task_view = Blueprint('tasks', __name__)


@task_view.route('/api/New', methods=['POST'])
def api_new():
    pass


@task_view.route('/api/Remove', methods=['GET'])
def api_remove():
    pass


@task_view.route('/api/Start', methods=['GET'])
def api_start():
    pass


@task_view.route('/api/StartAll', methods=['GET'])
def api_start_all():
    pass


@task_view.route('/api/Stop', methods=['GET'])
def api_stop():
    pass


@task_view.route('/api/StopAll', methods=['GET'])
def api_stop_all():
    pass


@task_view.route('/api/Modify', methods=['POST'])
def api_modify():
    pass


@task_view.route('/api/Query', methods=['GET'])
def api_query():
    pass


@task_view.route('/api/Status', methods=['GET'])
def api_status():
    pass


@task_view.route('/api/StatusAll', methods=['GET'])
def api_status_all():
    pass


@task_view.route('/api/ChangeMuxerOut', methods=['GET'])
def api_change_muxer_out():
    pass


@task_view.route('/api/ChangeStreamer', methods=['GET'])
def api_change_streamer():
    pass
