<center><h1>git まとめ</h1></center>

Index
===
- [Index](#index)
- [コミッタ等書き換え](#%e3%82%b3%e3%83%9f%e3%83%83%e3%82%bf%e7%ad%89%e6%9b%b8%e3%81%8d%e6%8f%9b%e3%81%88)

# コミッタ等書き換え
```bash
git filter-branch -f --commit-filter '
  if [ "$GIT_AUTHOR_NAME" = "author_name" ];
    then
        GIT_COMMITTER_NAME="committer_name";
        GIT_AUTHOR_NAME="author_name";
        GIT_COMMITTER_EMAIL="committer_email";
        GIT_AUTHOR_EMAIL="author_email";
        git commit-tree "$@";
    fi'  HEAD
```