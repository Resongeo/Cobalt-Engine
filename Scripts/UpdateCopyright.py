import os

search_text = '// Copyright (c) 2025 Somogyvári Benedek'
replace_text = '// Copyright (c) 2026 Somogyvári Benedek'

root_dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..')
engine_dir = os.path.join(root_dir, 'Engine')
editor_dir = os.path.join(root_dir, 'Editor')
tests_dir = os.path.join(root_dir, 'Tests')

def replace_recursively(path):
    for root, dirs, files in os.walk(path):
        for file in files:
            filename = os.path.join(root, file)
            print('[REPLACING]', filename)
            with open(filename, 'r') as file:
                data = file.read()
                if data.find(search_text):
                    data = data.replace(search_text, replace_text)

            with open(filename, 'w') as file:
                file.write(data)

replace_recursively(engine_dir)
replace_recursively(editor_dir)
replace_recursively(tests_dir)
