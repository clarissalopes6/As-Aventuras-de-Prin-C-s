# As Aventuras de Prin-C-s

Um jogo desenvolvido em C utilizando a biblioteca Raylib.

## Requisitos

- [Raylib](https://www.raylib.com/)
- Compilador C (gcc ou clang)
- Make

## Instalação das Dependências

Você pode instalar as dependências automaticamente com:

```sh
make deps
```

Se preferir instalar manualmente, siga as instruções abaixo conforme seu sistema operacional.

### Linux (Debian/Ubuntu)

```sh
sudo apt-get update
sudo apt-get install build-essential pkg-config libraylib-dev libgl1-mesa-dev
```

### Linux (Arch)

```sh
sudo pacman -Sy base-devel raylib
```

### Linux (Fedora)

```sh
sudo dnf install @development-tools raylib-devel
```

### macOS

Instale o [Homebrew](https://brew.sh/) e depois:

```sh
brew install raylib
```

### Windows

Recomenda-se usar [MSYS2](https://www.msys2.org/):

```sh
pacman -Syu mingw-w64-x86_64-raylib
```

## Compilação

Para compilar o jogo, execute:

```sh
make
```

## Executando o Jogo

Após compilar, rode:

```sh
make run
```
ou
```sh
./jogo
```

## Limpeza dos arquivos de build

```sh
make clean
```
### ScreenCast da Gameplay
[Screencast](https://youtu.be/K7HGJKds8LU)


## Créditos

Desenvolvido por Rafael Padilha, Clarissa Lopes e Pedro Abner.
