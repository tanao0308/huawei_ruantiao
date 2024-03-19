import subprocess
import json

map_num = 8
seeds_num = 1

totScore = 0
for i in range(map_num):
    for j in range(seeds_num):
        PreliminaryJudge_path = "D:/Desktop/huawei_ruantiao/WindowsReleasev1.2/PreliminaryJudge"
        map_command = " -m D:/Desktop/huawei_ruantiao/WindowsReleasev1.2/maps/map{}.txt".format(i+1)
        main_path = " \"D:/Desktop/huawei_ruantiao/build/main.exe\""
        command_to_execute = PreliminaryJudge_path + map_command + main_path
        # print(command_to_execute)
        result = subprocess.run(command_to_execute, shell=True, capture_output=True, text=True)
        print("map{}".format(i+1), result.stdout)
        totScore += json.loads(result.stdout)["score"]

print("Mean Score:", totScore/map_num)
