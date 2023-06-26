# Version-Control-System
Version control system developed with C programming language

# Quickstart
```bash
git clone 
cd Version-Control-System/src
```

# Usage
```bash
clang vcs.c actions.c data_structures.c files.c parser.c utils.c validations.c -o versionador
./versionador iniciar
./versionador adiciona <path01> <path02> <path03> <path04> <path05>
./versionador registra <mensagem> 
./versionador log  
./versionador log --conteudo
./versionador mostrar <commit>
./versionador mudar <commit>
```