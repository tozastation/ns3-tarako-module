# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('tarako', ['core'])
    module.source = [
        'model/csv.cc',
        'model/logger.cc',
        'model/tracer.cc',
        'model/util.cc',
        'model/group_node.cc',
        ]

    module_test = bld.create_ns3_module_test_library('tarako')
    module_test.source = [
        'test/tarako-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'tarako'
    headers.source = [
        'model/csv.h',
        'model/garbage_station.h',
        'model/node_info.h',
        'model/const.h',
        'model/group.h',
        'model/logger.h',
        'model/node_payload.h',
        'model/tracer.h',
        'model/util.h',
        'model/group_node.h'
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

