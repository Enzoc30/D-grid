#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <SFML/Graphics.hpp>
#include <filesystem>

double resize_factor = 40;
double initial_x_min = 35, initial_x_max = 75, intial_y_min = 50, intial_y_max = 90, initial_radius = 20;
const double X = 800, Y = 600;

double distanceQ(const std::pair<double, double>& p1, const std::pair<double, double>& p2) {
    return std::sqrt(std::pow(p2.first - p1.first, 2) + std::pow(p2.second - p1.second, 2));
}

// basic range query
std::vector<std::pair<double, double>> rangeQueryRectangular(const std::vector<std::pair<double, double>>& points,
                                                             const std::pair<double, double>& topLeft,
                                                             const std::pair<double, double>& bottomRight) {
    std::vector<std::pair<double, double>> result;

    for(const auto& point : points) {
        double x = point.first;
        double y = point.second;

        if(x >= topLeft.first && x <= bottomRight.first && y >= topLeft.second && y <= bottomRight.second) {
            result.push_back(point);
        }
    }

    return result;
}

std::vector<std::pair<double, double>> generateEarthCoordinates(int numCoordinates) {
    std::random_device rd;
    std::mt19937 gen(rd());
    // Latitude (-90, 90)
    std::uniform_real_distribution<double> latDistribution(0, 90.0);
    // Longitude (-180, 180)
    std::uniform_real_distribution<double> lonDistribution(0, 180.0);
    std::vector<std::pair<double, double>> coordinates;

    for(int i = 0; i < numCoordinates; ++i){
        double latitude = latDistribution(gen);
        double longitude = lonDistribution(gen);
        coordinates.push_back(std::make_pair(longitude, latitude));
    }

    return coordinates;
}

void drawRectangle(double x, double y){
    sf::RectangleShape rectangle(sf::Vector2f(x, y));
    rectangle.setPosition(0, 0);
    rectangle.setOutlineThickness(2.0f);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setFillColor(sf::Color::Transparent);
}

void drawPoints(sf::RenderWindow& window, const std::vector<std::pair<double, double>>& points) {
    std::filesystem::path interfacePath = std::filesystem::current_path().parent_path() / "Interface";
    sf::Texture taxiTexture;
    if(!taxiTexture.loadFromFile(interfacePath / "taxi.png")) {
        std::cout << "Taxi image failure :c" << std::endl;
        return;
    }
    for (const auto& coord : points) {
        // Scale coordinates to fit within the window while maintaining proportion
        // float x = static_cast<float>(coord.first + 180.0) * window.getSize().x / 360.0;
        // float y = static_cast<float>(coord.second + 90.0) * window.getSize().y / 180.0;

        float x = static_cast<float>(window.getSize().x / (initial_x_max - initial_x_min)*(coord.first - initial_x_min));
        float y = static_cast<float>(window.getSize().y / (intial_y_max - intial_y_min)*(coord.second - intial_y_min));

        // Draw a small circle for each point
        sf::CircleShape pointShape(2.0);
        pointShape.setPosition(x, y);
        pointShape.setTexture(&taxiTexture);
        pointShape.setFillColor(sf::Color::Blue);

        window.draw(pointShape);
    }
}

void drawInputBoxes(sf::RenderWindow& window, std::vector<sf::Text>& labels) {
    sf::Vector2f boxSize(150.0, 30.0);
    float margin = 10.0;
    float startY = window.getSize().y - boxSize.y - margin;
    std::filesystem::path interfacePath = std::filesystem::current_path().parent_path() / "Interface";

    sf::Color yellowPastel(255, 255, 153);
    sf::Color greenPastel(144, 238, 144);
    sf::Font font;
    if(!font.loadFromFile(interfacePath / "TirtoWritterRegular-Eajrl.ttf")) {
        // Handle loading failure
        std::cout << "Font loading failure :c" << std::endl;
        return;
    }
    std::vector<std::string> notes = {"P1", "P2", "tf"};

    for (int i = 0; i < 3; ++i) {
        // Etiqueta
        sf::Text label;
        label.setFont(font);
        label.setString(notes[i]);
        // label.setString("Label " + std::to_string(i + 1));
        label.setCharacterSize(16);
        label.setFillColor(sf::Color::Black);
        label.setPosition(margin + i * (boxSize.x + margin), startY - 20.0);
        labels.push_back(label);
        window.draw(label);

        // Casilla de entrada
        sf::RectangleShape inputBox(boxSize);
        inputBox.setPosition(margin + i * (boxSize.x + margin), startY);
        inputBox.setFillColor((i % 2 == 0) ? yellowPastel : greenPastel);
        window.draw(inputBox);
    }
}

int main(){
    std::filesystem::path interfacePath = std::filesystem::current_path().parent_path() / "Interface";
    auto coordinates = generateEarthCoordinates(1000);
    auto query_result = rangeQueryRectangular(coordinates, {initial_x_min, intial_y_max}, {initial_x_max, intial_y_min});
    sf::RenderWindow window(sf::VideoMode(800, 600), "Taxis all around the world!!!");
    std::vector<sf::Text> labels;

    sf::Texture backgroundImage;
    if(!backgroundImage.loadFromFile(interfacePath / "purple.jpg")) {
        std::cout << "Street image failure :c" << std::endl;
        return -1;
    }

    sf::Font font;
    if(!font.loadFromFile(interfacePath / "TirtoWritterRegular-Eajrl.ttf")) {
        // Handle loading failure
        std::cout << "Font loading failure :c" << std::endl;
        return -1;
    }

    sf::Sprite backgroundSprite(backgroundImage);
    // backgroundSprite.setColor(sf::Color(255, 255, 255, 128));

    sf::View view;
    view.setSize(800, 600);
    view.setCenter(400, 300);
    window.setView(view);
    sf::String playerInput;
    sf::Text playerText;

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
            // Zooming options
            if(event.type == sf::Event::MouseWheelScrolled){
                if(event.mouseWheelScroll.delta > 0){
                    view.zoom(0.9f);
                }
                else if(event.mouseWheelScroll.delta < 0){
                    view.zoom(1.1f);
                }
                window.setView(view);
            }

            if(event.type == sf::Event::TextEntered) {
                for(auto& label : labels) {
                    // Asegurarse de que la etiqueta tenga el foco antes de permitir la entrada
                    if(label.getGlobalBounds().contains(sf::Vector2f(event.text.unicode, event.text.unicode))) {
                        // Concatenar el carácter a la etiqueta
                        label.setString(label.getString() + static_cast<char>(event.text.unicode));
                    }
                }
            }

            if(event.type == sf::Event::TextEntered){
                playerInput +=event.text.unicode;
                playerText.setString(playerInput);
            }
        }

        window.clear();

        window.draw(backgroundSprite);

        // Drawing taxis
        drawPoints(window, coordinates);
        drawInputBoxes(window, labels);
        /*
        for(const auto& coord : query_result){
            float x = static_cast<float>(coord.second + 180.0) * 2.0;
            float y = static_cast<float>(coord.first + 90.0) * 2.0;

            // Taxi
            sf::RectangleShape taxiShape(sf::Vector2f(4.0, 2.0));
            taxiShape.setPosition(x, y);
            taxiShape.setTexture(&taxiTexture);
            window.draw(taxiShape);

            // sf::Text coordinatesText;
            // coordinatesText.setFont(font);
            // coordinatesText.setString("(" + std::to_string(coord.second) + ", " + std::to_string(coord.first) + ")");
            // coordinatesText.setCharacterSize(0.5);
            // coordinatesText.setScale(0.5f, 0.5f);
            // coordinatesText.setCharacterSize(3);
            // coordinatesText.setFillColor(sf::Color::Red);
            
            // Adjusted the position further to be above the taxi shape
            // coordinatesText.setPosition(x, y);

            // window.draw(coordinatesText);
        }*/

        window.display();
    }
}