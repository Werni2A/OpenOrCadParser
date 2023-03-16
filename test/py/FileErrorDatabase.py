import os
import yaml
from typing import Any, List
from yaml.loader import SafeLoader

from git import Git, Repo
from pydantic import BaseModel


class RepoFile(BaseModel):
    path:    str
    errors:  int
    options: Any


class Repository(BaseModel):
    author:  str
    commit:  str
    files:   List[RepoFile]
    project: str
    url:     str


class Repositories(BaseModel):
    repositories: List[Repository]


class FileErrorDatabase:

    def __init__(self):
        self.data: Repositories = Repositories(repositories=[])

    def readDatabaseFile(self, path_input_file: str) -> Repositories:
        with open(path_input_file, 'r') as f:
            self.data: Repositories = Repositories(**list(yaml.load_all(f, Loader=SafeLoader))[0])

    def writeDatabaseFile(self, path_output_file: str) -> None:
        with open(path_output_file, 'w') as f:
            yaml.dump(self.data.dict(), f)

    def isRepoInDb(self, repo: Repository) -> bool:
        for db_repo in self.data.repositories:
            if db_repo.dict() == repo.dict():
                return True

        return False

    def addRepo(self, repo: Repository) -> None:
        if self.isRepoInDb(repo):
            raise Exception('Repository is already present in the database, can not add it!')

        self.data.repositories += [repo]

    def getRepoByUrl(self, url: str) -> Repository:
        for repo in self.data.repositories:
            if repo.url == url:
                return repo

        raise Exception(f'Did not find repository with URL `{url}`')

    def getRepoByNames(self, author: str, project: str) -> Repository:
        for repo in self.data.repositories:
            if repo.author == author and repo.project == project:
                return repo

        raise Exception(f'Did not find repository from author `{author}` and project `{project}`')

    def getFileByPath(self, repo: Repository, path_file: str) -> RepoFile:
        if os.path.isabs(path_file):
            raise Exception('You should provide an relative path not an absoulte one!')

        for file in repo.files:
            if file.path == path_file:
                return file

        raise Exception(f'Did not find repository file with path `{path_file}`')

    def isFileInRepo(self, repo: Repository, file: RepoFile) -> bool:
        for repo_file in repo.files:
            if repo_file.path == file.path:
                return True

        return False

    def addFileToRepo(self, repo: Repository, file: RepoFile) -> None:
        if self.isFileInRepo(repo, file):
            raise Exception('File is already present in repository, can not add it!')

        repo.files += [file]

    def updateErrorCounter(self, file: RepoFile, error_count: int) -> None:
        """
        Updates error counter in case the new one is smaller than the existing one.
        """

        if error_count < file.errors:
            print(f'Reducing error count from {file.errors} to {error_count}.')
            file.errors = error_count
        elif error_count > file.errors:
            print(f'Increased error count from {file.errors} to {error_count}!')

    def cloneRepoTo(self, repo: Repository, base_path: str) -> None:
        """
        Cloens the repository to base_path/author/project
        """

        export_dir = os.path.join(base_path, repo.author, repo.project)

        if os.path.exists(export_dir):
            print(f'Directory `{export_dir}` already exists. Skipping repository download!')
            return

        print(f'Cloning {repo.url} to {export_dir} ...')

        try:
            fs_repo = Repo.clone_from(repo.url, export_dir)
        except:
            raise Exception(f'Failed cloning repository from `{repo.url}`!')

        # Commit differs from currently downloaded master
        if str(fs_repo.head.commit) != repo.commit:
            req_commit = None  # Required commit from database

            # Find correct commit object
            for c in fs_repo.iter_commits():
                if repo.commit == str(c):
                    req_commit = c
                    break

            if req_commit is None:
                raise Exception('Did not find commit `{repo.commit}` in repository history!')

            print(f'Repository is set to commit {fs_repo.head.commit}')
            print(f'Changing repository commit to {req_commit}')

            g = Git(export_dir)
            g.checkout(req_commit)

            if str(fs_repo.head.commit) != repo.commit:
                raise Exception('Checkout did not work!')

    def cloneReposTo(self, repo_dir: str) -> None:
        for repo in self.data.repositories:
            self.cloneRepoTo(repo, repo_dir)

    def getFsRepoPath(self, repo: Repository, base_path: str) -> str:
        """
        Retrieve absolute path to repository on filesystem.
        """

        return os.path.join(base_path, repo.author, repo.project)

    def getFsRepoCommitHash(self, repo: Repository, base_path: str) -> Any:
        """
        Retrieve currently check out commit hash of repository on filesystem.
        """

        fs_repo = Repo(self.getFsRepoPath(repo, base_path))

        return fs_repo.head.commit


if __name__ == '__main__':

    path_yaml = 'test_db.yaml'

    db = FileErrorDatabase()
    db.readDatabaseFile(path_yaml)

    my_file = RepoFile(path='/foo/bar', errors=666)
    my_repo = Repository(author='Max', commit='a74fa', files=[], project='test', url='www.page.com')
    db.addRepo(my_repo)
    my_repo = db.getRepoByUrl('www.page.com')
    db.addFileToRepo(my_repo, my_file)

    db.writeDatabaseFile(path_yaml)
