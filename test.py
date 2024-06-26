import os
import json
import subprocess
import numpy as np
from datetime import datetime

map_num = 8
seed_num = 3

def work(i, j): # map_i seed_j 
    PreliminaryJudge_path = os.path.join(os.getcwd(), "WindowsRelease/SemiFinalJudge")
    wait_command = " -f 0"
    map_command = " -m WindowsRelease/maps/map{}.txt".format(i+1)
    replay_command = " -r {}_map{}_seed{}.rep".format(datetime.now().strftime('%Y-%m-%d.%H.%M.%S'),i+1,j+1)
    seed_command = " -s {}".format(j+1)
    log_command = " -l DBG"
    output_command = " -d debug.txt"
    main_path = " \"build/main.exe\""
    command_to_execute = PreliminaryJudge_path + wait_command + map_command + replay_command + seed_command + log_command + output_command + main_path
    print(command_to_execute)
    result = subprocess.run(command_to_execute, shell=True, capture_output=True, text=True)
    print("map{}, seed{} ->".format(i + 1, j + 1), result.stdout)
    score = json.loads(result.stdout)["score"]
    return score


work(4, 0)
exit()

# i, mean_score = 0, 0
# for j in range(seed_num):
#     mean_score += work(i, j) / seed_num
# print(mean_score)
# exit()

j, mean_score = 2, 0
for i in range(map_num):
    mean_score += work(i, j) / map_num
print(mean_score)
exit()

map_scores = []
for i in range(map_num):
    mean_score = 0
    for j in range(seed_num):
        mean_score += work(i, j) / seed_num
    map_scores.append(mean_score)

print(map_scores)
print("Mean Score:", np.mean(map_scores))
