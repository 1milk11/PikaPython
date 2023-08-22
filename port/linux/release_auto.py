import os
import git
import subprocess
from release_helper import *

repo = git.Repo(REPO_PATH)
commit_head = repo.head.commit.hexsha
pkgReleases = PackageReleaseList(PACKAGE_RELEASE_PATH)

for folder in os.listdir(LINUX_PACKAGE_PATH):
    # skip PikaStdLib
    if folder == "PikaStdLib" or folder == "GTestTask" or folder == "TemplateDevice":
        continue
    # call `bash pkg-push $folder`
    cmd = f"./pkg-push.sh {folder}"
    output = subprocess.check_output(["bash", "-c", cmd])
    print(output.decode())

# for each folder in package, run the following command
for folder in os.listdir(PACKAGE_PATH):
    # skip PikaStdLib
    if folder == "PikaStdLib" or folder == "GTestTask" or folder == "TemplateDevice":
        continue
    if os.path.isdir(PACKAGE_PATH + "/" + folder):
        # check git diff
        diff = repo.git.diff(
            "HEAD",
            pkgReleases.latestCommit(folder),
            WORK_DIR + "/" + PACKAGE_PATH + "/" + folder
        )
        if diff != '':
            newVersion = pkgReleases.versionRelease(
                folder,
                VersoinType.PATCH,
                commit_head
            )
            print(f"Changes detected: {folder} --> {newVersion}")

pkgReleases.dump(PACKAGE_RELEASE_PATH)
exit()
