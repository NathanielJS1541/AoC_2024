# Advent of Code Template
A template repository for solutions to the yearly [Advent of Code](https://adventofcode.com/) challenges.

```
                                      _.--"""--,
                                    .'          `\
  .-""""""-.                      .'              |
 /          '.                   /            .-._/
|             `.                |             |
 \              \          .-._ |          _   \
  `""'-.         \_.-.     \   `          ( \__/
        |             )     '=.       .,   \
       /             (         \     /  \  /
     /`               `\        |   /    `'
     '..-`\        _.-. `\ _.__/   .=.
          |  _    / \  '.-`    `-.'  /
          \_/ |  |   './ _     _  \.'
               '-'    | /       \ |
                      |  .-. .-.  |   M E R R Y
                      \ / o| |o \ /
                       |   / \   |   C H R I S T M O O S E !
                      / `"`   `"` \
                     /             \
                    | '._.'         \
                    |  /             |
                     \ |             |
                      ||    _    _   /
                      /|\  (_\  /_) /
                      \ \'._  ` '_.'
                       `""` `"""`
```

## Contents
- [Getting Started](#getting-started)
  - [Cloning the Repo](#cloning-the-repo)
  - [Contribution Guidelines](#contribution-guidelines)
- [Contributors](#contributors)

## Getting Started
### Cloning the repo
Since this is a private repo, you will need to authenticate `git` with your GitHub account. The easiest way to do this is to use [GitHub Desktop](https://desktop.github.com/download/), as it should handle this for you.

Alternatively, create a [Personal Access Token](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens), or set up [SSH](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/about-ssh).

After this is set up, you can either `clone` this repo using GitHub desktop, using the command line with `git clone https://github.com/NathanielJS1541/AoC_Template.git`, or if you're using SSH do `git clone git@github.com:NathanielJS1541/AoC_Template.git`.

### Contribution Guidelines
This should just prevent treading on each others toes, and loss of work etc.

- Work in your own branch, and create a [Pull Request](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request) to merge your work into main periodically. You can complete this PR yourself, but it makes it easy to identify what was added by who.
- Avoid rebasing. This has the potential to create confusing scenarios for others, or overwrite others work if they have based work on a commit that you rebase.
- Do not `git push --force`! People can and will lose work this way.
- Create your own named folder inside this repo to reflect your name, for example `bob`. From there you can structure it however you like, and create your own README.md that can be linked to the [contributors](#contributors) section below.
- Use a `.gitignore` file locally within your own folder. `.gitignore` files are applied to the directory they are in, and any subdirectories below that. Because there will be lots of additions to the `.gitignore` file with each language people use, having them within your own folder means they can be customised to the languages you are using much easier. GitHub maintains a [repo of .gitignore templates](https://github.com/github/gitignore) if you need some inspiration.
- There is a `.gitignore` in the root of the repo, which contains the `.gitignore` templates for various common IDE's (Visual Studio, VSCode, JetBrains etc.). If your IDE needs some files ignored in the `.gitignore`, it should be added here to ensure any dotfiles your IDE generates when you open the repo are ignored, as these are usually placed in the root. 

## Contributors
Here you can get links to the READMEs within everyone's folder.
- [Example](./Example/README.md)
