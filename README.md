# Sense File e(X)plorer (RG26-sensefx)
[![not found](examples_img/yt-thumbnail.png?raw=true)](https://www.youtube.com/watch?v=HEGZIUWtsqQ)

**YouTube demo snimak:** [link](https://www.youtube.com/watch?v=HEGZIUWtsqQ)

**MP4 demo snimak:** Postoji i demo snimak u mp4 formatu rezolucije 800x600 koji se nalazi u direktorijumu projekta examples_video.

## Requirements
   Linux, [g++ (c++17)](https://gcc.gnu.org/), [OpenGL](https://www.opengl.org/), [glut](https://www.opengl.org/resources/libraries/glut/), [glm](https://glm.g-truc.net/0.9.8/index.html)

## Instalacija
### Ubuntu/Debian
      sudo apt-get install freeglut3-dev libglm-dev
### Arch/Manjaro Linux
      sudo pacman -Sy freeglut glm

## Kompilacija
Shell programom doći u direktorijum projekta i ukucati: `make`

## Primer pokretanja programa
U direktorijumu projekta ukucati:
     `./senseFX demo_directory`

## Kontrole
### Tastatura
| Key              | funkcija                         |
| ---------------- |:--------------------------------:|
| w,s,a,d          | Kretanje                         |
| h,j,k,l          | Rotiranje kamere                 |
| e                | Ulazak u direktorijum            |
| space            | Skok                             |
| f                | Uključivanje/Isključivanje lampe |

**Miš** - Look around (rotiranje kamere)


## NAPOMENA
* Projekat je još uvek u fazi razvoja, neke funkcionalnosti još uvek nisu implementirane, kod je potrebno dodatno "**isčistiti**" i ulepšati tj. postoje mesta za "**zlim kodom**". Takođe velika je verovatnoća da postoje i određeni bug-ovi.
* Ovaj projekat je moj prvi projekat i susret sa programskim jezikom C++, do sada nisam imao iskustva sa ovim jezikom ali jesam sa drugim objektno-orijentisanim jezicima.

## Preuzet kod (tuđi):
* Kod za učitavanje .bmp fajlova (image.h i image.cpp): [imgloader](http://poincare.matf.bg.ac.rs/~ivan/files/rg/vezbe/07/32_texture.tar.bz2)
* Kod za učitavanje .obj fajlova (objloader.h i objloader.cpp): [objloader](https://github.com/huamulan/OpenGL-tutorial/blob/master/common)

## INFO
Za više informacija posetiti Wiki sekciju projekta: [Wiki](https://github.com/MATF-RG17/RG026-sensefx/wiki)
