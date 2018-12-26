# -*- coding: utf-8 -*-
# Author: thansky


from .base import db
from uuid import uuid1
import json


class GpuTaskModel(db.Model):
    __table__name = 'GpuTask'

    # gtid = db.Colum(db.Integer, primary_key=True, autoincrement=True)
    tuuid = db.Column(db.String(64), index=True, unique=True, primary_key=True)
    gpuid = db.Column(db.String(16))

    def __init__(self, *, tuuid, gpuid):
        self.tuuid = tuuid
        self.gpuid = gpuid

    def __repr__(self):
        return '<GpuTaskModel %r>' % self.tuuid

    def __str__(self):
        return '<GpuTaskModel %s>' % self.tuuid


class TaskModel(db.Model):
    __table__name = 'Task'

    tuuid = db.Column(db.String(64), index=True, unique=True, primary_key=True)
    name = db.Column(db.String(128), index=True)
    kind = db.Column(db.String(64), index=True)
    content = db.Column(db.Text, index=True)

    def __init__(self, name, kind, content, *, task_id=None):
        self.name = name
        self.kind = kind
        self.content = content
        if not task_id:
            self.tuuid = "task_"+uuid1().hex

    def __repr__(self):
        return '<TaskModel %r>' % self.tuuid

    def __str__(self):
        return '<TaskModel %s>' % self.tuuid


class TaskDigest(object):

    def __init__(self):
        pass

    def __repr__(self):
        return 'TaskDigest({0.task_id!r},{0.task_name!r}\
                           {0.gpu_id!r},{0.task_kind!r},\
                           {0.inputs!r}, {0.outputs!r})'.format(self)

    def __str__(self):
        return '{0.task_id!s},{0.task_name!s}\
                {0.gpu_id!s},{0.task_kind!s},\
                {0.inputs!s}, {0.outputs!s}'.format(self)

    def to_json(self):
        j = {'TaskID': self.task_id, 'TaskName': self.task_name,
             'GpuID': self.gpu_id, 'Kind': self.kind,
             'Inputs': self.inputs, 'Outputs': self.outputs}
        return json.dump(j)


class Task(object):
    digest = None
    model = None

    def __init__(self, task_str, gpu_id, task_kind):
        pass

    def __analyse__():
        return False

    def __str__(self):
        return '{}'.format(self)

    def __repr__(self):
        return 'Task{}'.format(self)


class TaskStorage(object):
    task_list = list()

    @staticmethod
    def add(name, kind, content, *, task_id=None):
        task = Task(name, kind, content, task_id=None)
        db.session.add(task)
        db.commit()
        TaskStorage.task_list.append(task)

    @staticmethod
    def remove(tuuid):
        TaskStorage.task_list = [task for task in TaskStorage.task_list
                                 if task.tuuid != tuuid]

    @staticmethod
    def find(tuuid):
        for task in TaskStorage.task_list:
            if task.tuuid == tuuid:
                return task
        return None
