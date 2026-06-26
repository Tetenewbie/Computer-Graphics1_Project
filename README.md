🎓 Hochschule Hannover – Computergrafik 1

Dieses Projekt ist ein Computergrafik-Projekt für den Kurs Computergrafik 1. Ziel des Projekts war die Entwicklung einer interaktiven 3D-Szene mit C und OpenGL, um grundlegende Techniken der Grafikprogrammierung praktisch umzusetzen.


👥 Teammitglieder

    Artur Sanamyan artur.sananyan@stud.hs-hannover.de

    Kevin Hey kevin.hey@stud.hs-hannover.de

    Boris Schwarz boris.schwarz@stud.hs-hannover.de 



   Straßenszenen-Projekt

   Ein OpenGL-Grafikprojekt in C, das eine dynamische Straßenszene mit einem fahrenden Auto, einer Skybox und verschiedenen 3D-Objekten darstellt. Das Projekt demonstriert grundlegende Konzepte der Computergrafik, darunter das Laden  von 3D-Modellen, Texture Mapping, Kamerabewegung sowie Shader-Management.


🚀 Funktionen

        Dynamische Fahrzeugbewegung: Steuerung eines 3D-Automodells über die Tastatur.
        Skybox: Immersive 360-Grad-Umgebungsdarstellung.
        Beleuchtung: Einfache Phong-Beleuchtung mit diffusen und betrachtungsabhängigen Anteilen.
        Objekt-Rendering: Unterstützung für das Laden von .obj-Modellen und die Anwendung von Texturen.
        Transparenz: Alpha-Blending für Wolkeneffekte.
        Reaktionsfähige Kamera: Kamera, die der Bewegung des Fahrzeugs folgt.



🛠 Voraussetzungen

Stellen Sie sicher, dass die folgenden Komponenten auf Ihrem System installiert sind (Linux wird empfohlen):

        GCC (Compiler)
        GLFW3 (Fensterverwaltung)
        GLEW (OpenGL-Erweiterungs-Loader)
        pkg-config (zur Verwaltung von Abhängigkeiten)


🏗 Build & Run

        Das Project finden sie auch im  https://github.com/swarfeya/Computer-Graphics1_Project oder https://github.com/Tetenewbie/Computer-Graphics1_Project.
        
        Das Projekt verwendet ein einfaches Makefile zur Build-Verwaltung.

        1.Repository klonen:
            . git clone https://github.com/YourUsername/Computer-Graphics1_Project.git
            . cd Computer-Graphics1_Project
            
        2.Projekt kompilieren:
              .make

        3.Programm ausführen:
            .make run    

        4.Aufräumen:
            .make clean

🎮 Steuerung

        * W → Beschleunigen (vorwärts fahren)   
        * S → Bremsen / Rückwärts fahren
        * Pfeiltaste links → Kamera nach links schwenken
        * Pfeiltaste rechts → Kamera nach rechts schwenken




💡 Imports/Hilfe

        stb_image für die Bildladefunktionen
        https://www.textures.com/library
        https://learnopengl.com/
        Computergrafik 1, SoSe 2026, Allerkamp



![Game Screenshot](screenshots/car.png)
        

                
