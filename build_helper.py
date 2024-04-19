#!/usr/bin/env python3

import os
import shutil
import zipfile
import argparse
import hashlib

def extract_source(in_zip_path, out_src_path):
    "depress source code form release package"
    print('Extracting zipped release package...')
    f = zipfile.ZipFile(in_zip_path, "r")
    f.extractall(path=out_src_path)
    old_src_dir = out_src_path + "/mindspore-v2.1.0/"
    new_src_dir = out_src_path + "/source/"
    os.rename(old_src_dir, new_src_dir)
    print("Done extraction.")

def do_patch(patch_dir, target_dir):
    patches = [
        '0001-build-gn-c-api-for-OHOS.patch',
        '0002-train-and-build.patch',
        '0003-add-js-api.patch',
        '0004-cross-compile-ndkso-fp16-nnrt-train_capi.patch',
        '0005-micro-for-ohos.patch',
        '0006-remove-lite-expression-fix-double-loadso.patch',
        '0007-deobfuscator.patch',
        '0008-upgrade-flatbuffers-fix_crash.patch',
        '0009-npu-zero-copy.patch',
        '0010-micro-dynamic-shape-support-discrete-value.patch',
        '0011-fix-npu-infer-memory-leak-delete-liteGraph.patch',
        '0012-add-mindir-ops.patch',
        '0013-hiappevent.patch',
        '0014-DynamicQuant-strategy-optimization.patch',
        '0015-bugfix-for-cpu-kernel.patch',
        '0016-bugfix-for-argminmax-swish-int8-and-vad-asan.patch',
        '0017-bugfix-for-onnx-parser.patch',
        '0018-nnrt-litegraph-dequant.patch',
        '0019-adaper-NNCore-Api.patch',
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


def compute_md5(file):
    m = hashlib.md5()
    with open(file, 'rb') as f:
        m.update(f.read())
    return m.hexdigest()


def save_md5s(folder_path, out_path):
    files_list = []
    for file_name in os.listdir(folder_path):
        if (file_name.endswith(".patch")):
            files_list.append(file_name)

    os.makedirs(out_path, exist_ok=True)
    for pf in files_list:
        md5_path = os.path.join(out_path, pf.replace(".patch", ".md5"))
        with open(md5_path, 'w') as f:
            f.write(compute_md5(os.path.join(folder_path, pf)))


def md5_changed(patch_path, md5_path):
    if not os.path.exists(md5_path):
        return True
    patch_list = []
    md5_list = []
    for file_name in os.listdir(patch_path):
        if (file_name.endswith(".patch")):
            patch_list.append(file_name)
    for file_name in os.listdir(md5_path):
        if (file_name.endswith(".md5")):
            md5_list.append(file_name)
    if (len(patch_list) != len(md5_list)):
        return True

    for md5_file in md5_list:
        if not os.path.exists(os.path.join(patch_path, md5_file.replace(".md5", ".patch"))):
            return True
        with open(os.path.join(md5_path, md5_file), 'r') as f:
            origin_v = f.read().strip()
            if (origin_v != compute_md5(os.path.join(patch_path, md5_file.replace(".md5", ".patch")))):
                return True
    return False


def source_has_changed(out_src_path, patch_path, md5_path):
    if not os.path.exists(os.path.join(out_src_path, ".status")):
        print(".status not exist.")
        return True
    return md5_changed(patch_path, md5_path)


def main_work():
    parser = argparse.ArgumentParser(description="mindspore build helper")
    parser.add_argument('--in_zip_path')
    parser.add_argument('--out_src_path')
    parser.add_argument('--patch_dir')
    args = vars(parser.parse_args())

    in_zip_path = os.path.realpath(args['in_zip_path'])
    out_src_path = args['out_src_path']
    patch_dir = os.path.realpath(args['patch_dir'])

    md5_dir = os.path.join(out_src_path, "patches_md5")
    if source_has_changed(out_src_path, patch_dir, md5_dir):
        print("remove ", out_src_path)
        if os.path.exists(out_src_path):
            shutil.rmtree(out_src_path)
        save_md5s(patch_dir, md5_dir)

    if os.path.exists(os.path.join(out_src_path, ".status")):
        print("patch files not changed and " + os.path.join(out_src_path, ".status") + " exists.")
        return

    os.makedirs(out_src_path, exist_ok=True)
    out_src_path = os.path.realpath(out_src_path)

    extract_source(in_zip_path, out_src_path)

    do_patch(patch_dir, out_src_path + '/source/')

    create_status_file(out_src_path)


if __name__ == "__main__":
    main_work()

