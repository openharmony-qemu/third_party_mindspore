#!/usr/bin/env python3

import os
import shutil
import zipfile
import argparse

def extract_source(in_zip_path, out_src_path):
    "depress source code form release package"
    print('Extracting zipped release package...')
    f = zipfile.ZipFile(in_zip_path, "r")
    f.extractall(path=out_src_path)
    old_src_dir = out_src_path + "/mindspore-v1.8.1/"
    new_src_dir = out_src_path + "/source/"
    os.rename(old_src_dir, new_src_dir)
    print("Done extraction.")

def do_patch(patch_dir, target_dir):
    patches = [
        '0001-generate-schema-headers-manually.patch',
        '0002-generate-nnacl-simd-headers-manually.patch',
        '0003-implement-mindir-module-and-support-nnrt-delegate.patch',
        '0004-adapt-build-gn-and-provide-C-API-for-OHOS.patch',
        '0005-mindir-add-custom-op.patch',
        '0006-Support-converting-THIRDPARTY-model-in-MSLite.patch',
        '0007-support-third-party-model-in-mslite-runtime.patch',
        '0008-add-js-api.patch',
        '0009-adapt-nnrt-v2_0.patch',
        '0010-nnrt-delegate-supports-heterogeneous-predition.patch',
        '0011-replace-memcpy-with-memcpy_s-in-mindir.patch',
        '0012-add-secure-option.patch',
        '0013-train-0001.patch',
        '0014-train-0002.patch',
        '0015-train-0003.patch',
        '0016-fix-train-bug.patch',
        '0017-support-build-training-lib.patch',
        '0018-mindir-MaxPoolFusion-add-roundMode.patch',
        '0019-change-nnrt-as-external_dep-in-mindir.patch',
        '0020-add-ohos-js-callback-api-and-bugfix.patch',
        '0021-js-namespace-bugfix.patch',
        '0022-support-cross-compile-with-ohos-ndk.patch',
        '0023-js-info-bugfix.patch',
        '0024-js-info2-bugfix.patch',
        '0025-js-callback-bugfix.patch',
        '0026-js-buffer-add-bugfix.patch',
        '0027-js-buffer-leakage-bugfix.patch',
        '0028-modify-ext_dep-hilog.patch'
    ]

    cwd = os.getcwd()
    os.chdir(target_dir)
    print('Change dir to', os.getcwd())
    os.system('git init .')
    os.system('git add .; git commit -m "init"')
    
    for patch in patches:
        print('Applying ', patch, '...')
        ret = os.system('git apply ' + patch_dir + '/' + patch)
        if ret != 0:
            raise Exception("Apply patch {} failed.".format(patch))
        os.system('git add .; git commit -m "auto-apply ' + patch + '"')
        print('Done')
    os.chdir(cwd)

def create_status_file(out_src_path):
    f = open(out_src_path + '/.status', 'w')
    f.write('ok')
    f.close


def main_work():
    parser = argparse.ArgumentParser(description="mindspore build helper")
    parser.add_argument('--in_zip_path')
    parser.add_argument('--out_src_path')
    parser.add_argument('--patch_dir')
    args = vars(parser.parse_args())

    in_zip_path = os.path.realpath(args['in_zip_path'])
    out_src_path = args['out_src_path']
    patch_dir = os.path.realpath(args['patch_dir'])

    if os.path.exists(out_src_path):
        shutil.rmtree(out_src_path)

    os.mkdir(out_src_path)
    out_src_path = os.path.realpath(out_src_path)

    extract_source(in_zip_path, out_src_path)

    do_patch(patch_dir, out_src_path + '/source/')

    create_status_file(out_src_path)

if __name__ == "__main__":
    main_work()

