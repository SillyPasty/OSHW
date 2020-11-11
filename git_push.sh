eval $(ssh-agent -s)
ssh-add /root/.ssh/id_rsa_git
ssh -T git@github.com
git push origin master