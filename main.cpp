// =====================================================================
//  "Hola Mundo" desordenado - SFML 3
//  Las letras aparecen dispersas y rotadas por la pantalla. El usuario
//  las debe arrastrar con el mouse (click, mantener, soltar) hasta las
//  casillas objetivo para formar el texto "HOLA MUNDO".
//
//  Compilar (Linux/Mac, con SFML 3 instalado):
//    g++ -std=c++17 main.cpp -o holamundo -lsfml-graphics -lsfml-window -lsfml-system
//
//  Compilar (con CMake, ver CMakeLists.txt adjunto):
//    mkdir build && cd build && cmake .. && cmake --build .
//
//  IMPORTANTE: necesitas un archivo de fuente .ttf. El programa intenta
//  cargar automáticamente algunas fuentes comunes del sistema, o puedes
//  colocar un archivo llamado "arial.ttf" junto al ejecutable.
// =====================================================================

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <iostream>

// ---------------------------------------------------------------------
// Estructura que representa una letra "suelta" que se puede arrastrar
// ---------------------------------------------------------------------
struct Letra
{
    sf::Text texto;
    sf::Vector2f posObjetivo;   // posición correcta (casilla) a la que debe llegar
    sf::Vector2f offsetArrastre; // diferencia entre el mouse y la esquina del texto al agarrarla
    bool arrastrando = false;
    bool colocada = false;      // true cuando está encajada en su casilla

    Letra(const sf::Font& fuente, const std::string& caracter, unsigned tam, sf::Vector2f objetivo)
        : texto(fuente, caracter, tam), posObjetivo(objetivo)
    {
    }
};

// Intenta cargar una fuente desde varias rutas típicas del sistema
static bool cargarFuente(sf::Font& fuente)
{
    const std::vector<std::string> candidatos = {
        "arial.ttf",
        "Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSansBold.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "C:/Windows/Fonts/arialbd.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial Bold.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf"
    };

    for (const auto& ruta : candidatos)
    {
        if (fuente.openFromFile(ruta))
        {
            std::cout << "Fuente cargada: " << ruta << "\n";
            return true;
        }
    }
    return false;
}

int main()
{
    // -----------------------------------------------------------------
    // Ventana
    // -----------------------------------------------------------------
    sf::RenderWindow ventana(sf::VideoMode({900u, 600u}), "Hola Mundo Desordenado - SFML 3");
    ventana.setFramerateLimit(60);

    // -----------------------------------------------------------------
    // Fuente
    // -----------------------------------------------------------------
    sf::Font fuente;
    if (!cargarFuente(fuente))
    {
        std::cerr << "No se pudo cargar ninguna fuente. Coloca un archivo 'arial.ttf' "
                     "junto al ejecutable e intenta de nuevo.\n";
        return -1;
    }

    // -----------------------------------------------------------------
    // Palabra objetivo: "HOLA" + espacio + "MUNDO"
    // -----------------------------------------------------------------
    const std::string palabra = "HOLAMUNDO"; // sin espacio, el hueco se agrega visualmente
    const std::string texto = "RICARDOFUENTES";
    const unsigned tamLetra = 60;
    const unsigned tamtexto = 30;
    const float anchoCasilla = 60.f;
    const float altoLinea = 500.f; // altura (y) de las casillas objetivo
    const float espacioExtraDespuesDe = 4; // índice tras el cual va el espacio (después de HOLA)
    const float huecoEspacio = 35.f;

    // Calcular ancho total para centrar las casillas
    float anchoTotal = palabra.size() * anchoCasilla + huecoEspacio;
    float inicioX = (900.f - anchoTotal) / 2.f;

    // -----------------------------------------------------------------
    // Generador de números aleatorios para dispersar las letras
    // -----------------------------------------------------------------
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(40.f, 820.f);
    std::uniform_real_distribution<float> distY(60.f, 380.f);
    std::uniform_real_distribution<float> distRot(-40.f, 40.f);
    std::uniform_int_distribution<int> distColor(0, 4);

    const sf::Color paleta[5] = {
        sf::Color(231, 76, 60),   // rojo
        sf::Color(52, 152, 219),  // azul
        sf::Color(241, 196, 15),  // amarillo
        sf::Color(46, 204, 113),  // verde
        sf::Color(155, 89, 182)   // morado
    };

    // -----------------------------------------------------------------
    // Crear las letras dispersas + sus casillas objetivo
    // -----------------------------------------------------------------
    std::vector<Letra> letras;
    letras.reserve(palabra.size());

    std::vector<sf::RectangleShape> casillas;
    casillas.reserve(palabra.size());

    for (std::size_t i = 0; i < palabra.size(); ++i)
    {
        float extra = (i > espacioExtraDespuesDe) ? huecoEspacio : 0.f;
        sf::Vector2f objetivo(inicioX + i * anchoCasilla + extra, altoLinea);

        // Casilla (guía visual) en la posición objetivo
        sf::RectangleShape casilla(sf::Vector2f(anchoCasilla - 10.f, tamLetra + 10.f));
        casilla.setPosition(objetivo);
        casilla.setFillColor(sf::Color(255, 255, 255, 25));
        casilla.setOutlineColor(sf::Color(255, 255, 255, 120));
        casilla.setOutlineThickness(2.f);
        casillas.push_back(casilla);

        // Letra dispersa
        std::string caracter(1, palabra[i]);
        Letra letra(fuente, caracter, tamLetra, objetivo);
        letra.texto.setPosition({distX(gen), distY(gen)});
        letra.texto.setRotation(sf::degrees(distRot(gen)));
        letra.texto.setFillColor(paleta[distColor(gen)]);
        letra.texto.setStyle(sf::Text::Bold);
        letras.push_back(std::move(letra));
    }

    for (std::size_t i=0;i<texto.size(); ++i) {
        sf::Vector2f segundoObjetivo(inicioX + i * anchoCasilla, altoLinea + 100.f);
        sf::RectangleShape segundaCasilla(sf::Vector2f(anchoCasilla - 10.f, tamtexto + 10.f));
        segundaCasilla.setPosition(segundoObjetivo);
        segundaCasilla.setFillColor(sf::Color(255, 255, 255 , 25));
        segundaCasilla.setOutlineColor(sf::Color(255, 255, 255, 120));
        segundaCasilla.setOutlineThickness(2.f);
        casillas.push_back(segundaCasilla);
    }

    // -----------------------------------------------------------------
    // Textos de interfaz
    // -----------------------------------------------------------------
    sf::Text titulo(fuente, "Arrastra las letras con el mouse hasta su casilla", 22);
    titulo.setFillColor(sf::Color::White);
    titulo.setPosition({30.f, 15.f});

    sf::Text mensajeExito(fuente, "Correcto! Formaste HOLA MUNDO", 34);
    mensajeExito.setFillColor(sf::Color::Green);
    mensajeExito.setStyle(sf::Text::Bold);
    {
        sf::FloatRect bounds = mensajeExito.getLocalBounds();
        mensajeExito.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                                 bounds.position.y + bounds.size.y / 2.f});
        mensajeExito.setPosition({450.f, 560.f});
    }

    // Índice de la letra que se está arrastrando actualmente (-1 = ninguna)
    int indiceArrastrada = -1;
    const float distanciaEncaje = 28.f; // qué tan cerca hay que soltar para que encaje

    // -----------------------------------------------------------------
    // Bucle principal
    // -----------------------------------------------------------------
    while (ventana.isOpen())
    {
        while (const std::optional<sf::Event> evento = ventana.pollEvent())
        {
            if (evento->is<sf::Event::Closed>())
            {
                ventana.close();
            }
            else if (const auto* presion = evento->getIf<sf::Event::MouseButtonPressed>())
            {
                if (presion->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f posMouse(static_cast<float>(presion->position.x),
                                           static_cast<float>(presion->position.y));

                    // Buscar de "arriba hacia abajo" (última dibujada = encima)
                    for (int i = static_cast<int>(letras.size()) - 1; i >= 0; --i)
                    {
                        if (letras[i].texto.getGlobalBounds().contains(posMouse))
                        {
                            indiceArrastrada = i;
                            letras[i].arrastrando = true;
                            letras[i].colocada = false;
                            letras[i].offsetArrastre = letras[i].texto.getPosition() - posMouse;
                            break;
                        }
                    }
                }
            }
            else if (const auto* movimiento = evento->getIf<sf::Event::MouseMoved>())
            {
                if (indiceArrastrada != -1)
                {
                    sf::Vector2f posMouse(static_cast<float>(movimiento->position.x),
                                           static_cast<float>(movimiento->position.y));
                    letras[indiceArrastrada].texto.setPosition(posMouse + letras[indiceArrastrada].offsetArrastre);
                }
            }
            else if (const auto* suelta = evento->getIf<sf::Event::MouseButtonReleased>())
            {
                if (suelta->button == sf::Mouse::Button::Left && indiceArrastrada != -1)
                {
                    Letra& l = letras[indiceArrastrada];
                    l.arrastrando = false;

                    // ¿Quedó cerca de su casilla? -> encajar (snap)
                    sf::Vector2f diff = l.texto.getPosition() - l.posObjetivo;
                    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                    if (dist < distanciaEncaje)
                    {
                        l.texto.setPosition(l.posObjetivo);
                        l.texto.setRotation(sf::degrees(0.f));
                        l.colocada = true;
                    }

                    indiceArrastrada = -1;
                }
            }
        }

        // ---------------------------------------------------------
        // ¿Ya se completó la palabra?
        // ---------------------------------------------------------
        bool completo = true;
        for (const auto& l : letras)
        {
            if (!l.colocada) { completo = false; break; }
        }

        // ---------------------------------------------------------
        // Dibujar
        // ---------------------------------------------------------
        ventana.clear(sf::Color(30, 30, 40));

        ventana.draw(titulo);

        for (const auto& c : casillas)
            ventana.draw(c);

        for (const auto& l : letras)
            ventana.draw(l.texto);

        for (const auto & c : segundaCasilla)
            ventana.draw(c);

        if (completo)
            ventana.draw(mensajeExito);

        ventana.display();
    }

    return 0;
}