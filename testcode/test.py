import subprocess
import json
import numpy as np

map_num = 8
seed_num = 3


def work(i, j):
    PreliminaryJudge_path = "D:/Desktop/huawei_ruantiao/WindowsReleasev1.2/PreliminaryJudge"
    map_command = " -m D:/Desktop/huawei_ruantiao/WindowsReleasev1.2/maps/map{}.txt".format(i + 1)
    seed_command = " -s {}".format(j + 1)
    main_path = " \"D:/Desktop/huawei_ruantiao/build/main.exe\""
    command_to_execute = PreliminaryJudge_path + map_command + seed_command + main_path
    # print(command_to_execute)
    result = subprocess.run(command_to_execute, shell=True, capture_output=True, text=True)
    print("map{}, seed{} ->".format(i + 1, j + 1), result.stdout)
    score = json.loads(result.stdout)["score"]
    return score


# work(4, 1)
# exit()

# i, mean_score = 0, 0
# for j in range(seed_num):
#     mean_score += work(i, j) / seed_num
# print(mean_score)
# exit()

map_scores = []
for i in range(map_num):
    mean_score = 0
    for j in range(seed_num):
        mean_score += work(i, j) / seed_num
    map_scores.append(mean_score)

print(map_scores)
print("Mean Score:", np.mean(map_scores))
