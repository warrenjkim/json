FROM ubuntu:latest

ARG USER_NAME
ARG USER_UID
ARG USER_GID

RUN apt-get update && apt-get install -y \
    cmake make g++ libboost-all-dev gcovr gdb \
    zsh git curl sudo

RUN groupadd -o --gid $USER_GID $USER_NAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USER_NAME \
    && echo $USER_NAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USER_NAME \
    && chmod 0440 /etc/sudoers.d/$USER_NAME

USER $USER_NAME
WORKDIR $HOME

RUN mkdir -p $HOME/.config/zsh/plugins \
    && git clone https://github.com/zsh-users/zsh-autosuggestions $HOME/.config/zsh/plugins/zsh-autosuggestions \
    && git clone https://github.com/zsh-users/zsh-syntax-highlighting $HOME/.config/zsh/plugins/zsh-syntax-highlighting

RUN echo '\
git_prompt_info() {\n\
  local branch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null)\n\
  [[ -n $branch ]] && echo " branch:$branch"\n\
}\n\
\n\
alias ls="ls -lAFhN --color=auto --group-directories-first"\n\
alias rm="rm -v"\n\
alias cp="cp -v"\n\
alias mv="mv -v"\n\
\n\
setopt APPEND_HISTORY\n\
setopt EXTENDED_HISTORY\n\
setopt SHARE_HISTORY\n\
setopt HIST_IGNORE_DUPS\n\
setopt HIST_IGNORE_SPACE\n\
setopt GLOBDOTS\n\
\n\
autoload -Uz compinit && compinit\n\
\n\
zstyle ":completion:*" use-cache on\n\
zstyle ":completion:*" cache-path "$ZDOTDIR/.cache"\n\
zstyle ":completion:*" matcher-list "m:{a-zA-Z}={A-Za-z}" "r:|[._-]=* r:|=*" "l:|=* r:|=*"\n\
zstyle ":completion:*" list-colors "${(s.:.)LS_COLORS}"\n\
zstyle ":completion:*" menu select\n\
\n\
bindkey -e\n\
bindkey "^[[Z" autosuggest-accept\n\
\n\
# Load Plugins\n\
source $HOME/.config/zsh/plugins/zsh-autosuggestions/zsh-autosuggestions.zsh\n\
source $HOME/.config/zsh/plugins/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh\n\
\n\
PROMPT="%F{white}<%n%F{white}%B@%F{red}%Bdev-container%b%F{cyan}%~%F{green}$(git_prompt_info)%F{white}> %f"\n\
' > $HOME/.zshrc

WORKDIR $HOME/workspace

SHELL ["/bin/zsh", "-c"]
CMD ["zsh"]
