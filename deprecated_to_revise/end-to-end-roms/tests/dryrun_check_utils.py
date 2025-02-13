
import os
import sys
import subprocess
import yaml

def yaml_dic_check(yamlDicGold, yamlFileRead):
    with open(yamlFileRead, 'rt') as input_yaml:
        currDic = yaml.safe_load(input_yaml)
        print(f'===> Asserting...\n==> File: {yamlFileRead}')
        assert yamlDicGold==currDic
        print(f'==> Assertion passed.')
