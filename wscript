# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('tarako', ['core'])
    module.source = [
        'model/tarako.cc',
        'model/csv.cc',
        'helper/tarako-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('tarako')
    module_test.source = [
        'test/tarako-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'tarako'
    headers.source = [
        'model/tarako.h',
        'model/csv.h',
        'model/garbage_station.h',
        'helper/tarako-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

