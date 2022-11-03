#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

const int WIDTH = 600;
const int HEIGHT = 800;
const float GRAVITY = 1000;
int score = 0;
bool isDead = false;

class Player{
public:
    sf::Texture texture;
    sf::Sprite sprite;
    float dx=350.0, dy=50.0;
    bool movable = false;

    void setTexture(const std::string &name){
        texture.loadFromFile(name);
        sprite.setTexture(texture);
    }


    void setScale(float x, float y){
        sprite.setScale(x, y);
    }

    void Move(sf::Time elapsed){
        if (!movable || isDead){return;}

        if (sprite.getPosition().x + sprite.getTexture()->getSize().x * sprite.getScale().x < 20){
            sprite.move(sprite.getTexture()->getSize().x*sprite.getScale().x, 0);

            sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
            dx = std::abs(dx);
            score++;
        }
        if (sprite.getPosition().x + sprite.getTexture()->getSize().x * sprite.getScale().x> WIDTH-20){

            sprite.move(sprite.getTexture()->getSize().x*sprite.getScale().x, 0);
            sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
            dx = -std::abs(dx);
            score++;

        }

        if (sprite.getPosition().y < 32){
            dy = std::abs(dy);
        }

        if (sprite.getPosition().y + sprite.getTexture()->getSize().y * sprite.getScale().y > HEIGHT-32){
            dy = -std::abs(dy);
            dy = dy * 0.6;
        }else{
            dy += GRAVITY * elapsed.asMilliseconds()/1000;
        }

        sprite.setPosition(sprite.getPosition().x + dx * elapsed.asMilliseconds()/1000, sprite.getPosition().y + dy * elapsed.asMilliseconds()/1000);
    }

    void setPositionToCentre(){
        sprite.setPosition(WIDTH/2.0 - sprite.getTexture()->getSize().x * sprite.getScale().x, HEIGHT/2.0 - sprite.getTexture()->getSize().y * sprite.getScale().y);
    }

    void Jump(){
        dy = -500;

    }

    void Draw(sf::RenderWindow &w){
        w.draw(sprite);
    }

private:


};


class Spike{
public:
    sf::Sprite spike;
    bool visible = false;
    void changeVisibility(){
        visible= !visible;
    }



private:


};


bool isColissioned(sf::Sprite sp1, sf::Sprite sp2){
    float sp1_x1 = sp1.getPosition().x; // sp1 ax, cx
    float sp1_y1 = sp1.getPosition().y; // sp1 ay, cy
    float sp1_x2 = sp1_x1+ sp1.getTexture()->getSize().x * sp1.getScale().x; // sp1 bx, dx
    float sp1_y2 = sp1_y1+ sp1.getTexture()->getSize().y * sp1.getScale().y; // sp1 by, dy

    float sp2_x1 = sp2.getPosition().x; // sp2 ax, cx
    float sp2_y1 = sp2.getPosition().y; // sp2 ay, cy
    float sp2_x2 = sp2_x1+ sp2.getTexture()->getSize().x * sp2.getScale().x; // sp2 bx, dx
    float sp2_y2 = sp2_y1+ sp2.getTexture()->getSize().y * sp2.getScale().y; // sp2 by, dy


    // if sp2 inside sp1
    if (sp1_x1 <= sp2_x1 && sp1_x2 >= sp2_x2 && sp1_y1 <= sp2_y1 && sp1_y2 >= sp2_y2){
        return true;

    }

    // if sp1 inside sp2
    if (sp1_x1 >= sp2_x1 && sp1_x2 <= sp2_x2 && sp1_y1 >= sp2_y1 && sp1_y2 <= sp2_y2){
        return true;
    }

    // if sp2 a, b points inside sp1 (sp1 is bigger than sp2)
    if (sp2_x1 >= sp1_x1 && sp2_x2 <= sp1_x2 && sp2_y1 >= sp1_y1 && sp2_y1 <= sp1_y2){
        return true;
    }

    // if sp2 a,c points inside sp1 (sp1 is bigger than sp2)
    if (sp2_y1 >= sp1_y1 && sp2_y2 <= sp1_y2 && sp2_x1 >= sp1_x1 && sp2_x1 <= sp1_x2){
        return true;
    }

    // if sp2 c, d points inside sp1 (sp1 is bigger than sp2)
    if (sp2_x1 >= sp1_x1 && sp2_x2 <= sp1_x2 && sp2_y2 >= sp1_y1 && sp2_y2 <= sp1_y2){
        return true;
    }

    // if sp2 b, d points inside sp1 (sp1 is bigger than sp2)
    if (sp2_x2 >= sp1_x1 && sp2_x2 <= sp1_x2 && sp2_y1 >= sp1_y1 && sp2_y2 <= sp1_y2){
        return true;
    }

    // if sp1 a, b points inside sp2 (sp2 is bigger than sp1)


    // if sp1 b, d points inside sp2 (sp2 is bigger than sp1)


    // if sp1 c, d points inside sp2 (sp2 is bigger than sp1)


    // if sp1 a, c points inside sp2 (sp2 is bigger than sp1)


    // if sp1 d point inside sp2
    if (sp1_x2 >= sp2_x1 && sp1_x2 <= sp2_x2 && sp1_y2 >= sp2_y1 && sp1_y2 <= sp2_y2){
        return true;
    }

    // if sp1 c point inside sp2
    if (sp1_x1 >= sp2_x1 && sp1_x1 <= sp2_x2 && sp1_y2 >= sp2_y1 && sp1_y2 <= sp2_y2){
        return true;
    }

    // if sp1 b point inside sp2
    if (sp1_x2 >= sp2_x1 && sp1_x2 <= sp2_x2 && sp1_y1 >= sp2_y1 && sp1_y1 <= sp2_y2){
        return true;
    }

    // if sp1 a point inside sp2
    if (sp1_x1 >= sp2_x1 && sp1_x1 <= sp2_x2 && sp1_y1 >= sp2_y1 && sp1_y1 <= sp2_y2){
        return true;
    }



    return false;

}

int main()
{
    srand (time(NULL));
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Don't touch spikes!");
    window.setFramerateLimit(60);
    sf::Clock clock;

    sf::Texture sp_tx;
    sp_tx.loadFromFile("font.png");
    sp_tx.setSmooth(1);
    sf::Sprite font(sp_tx);


    sf::CircleShape midCircle;
    midCircle.setRadius(120);
    midCircle.setPosition(WIDTH/2 - 120, HEIGHT/2 -120);

    sf::Font textFont;
    textFont.loadFromFile("arial.ttf");

    sf::Text points;
    points.setFont(textFont);
    points.setFillColor(sf::Color(142, 142, 142));
    points.setCharacterSize(100);
    points.setPosition(WIDTH/2, HEIGHT/2 - 20);

    Player player;
    player.setTexture("ch.png");
    player.setScale(0.05, 0.05);
    player.setPositionToCentre();

    sf::Texture spikeTx;
    spikeTx.loadFromFile("spikes.png");
    spikeTx.setSmooth(1);
    sf::Texture spikeTx2;
    spikeTx2.loadFromFile("spikes2.png");
    spikeTx2.setSmooth(1);
    Spike spikes[20];
    for (int i = 0; i < 7; i++){
        spikes[i].spike.setTexture(spikeTx);
        spikes[i].spike.setScale(0.1, 0.1);
        //spikes[i].spike.setRotation(90);
    }
    for (int i = 7; i < 14; i++){
        spikes[i].spike.setTexture(spikeTx2);
        spikes[i].spike.setScale(0.1, 0.1);
        //spikes[i].spike.setRotation(90);
    }

    spikes[0].spike.setPosition(15, 55);
    spikes[1].spike.setPosition(15, 155);
    spikes[2].spike.setPosition(15, 255);
    spikes[3].spike.setPosition(15, 355);
    spikes[4].spike.setPosition(15, 455);
    spikes[5].spike.setPosition(15, 555);
    spikes[6].spike.setPosition(15, 655);

    spikes[7].spike.setPosition(540, 55);
    spikes[8].spike.setPosition(540, 155);
    spikes[9].spike.setPosition(540, 255);
    spikes[10].spike.setPosition(540, 355);
    spikes[11].spike.setPosition(540, 455);
    spikes[12].spike.setPosition(540, 555);
    spikes[13].spike.setPosition(540, 655);


    int prevScore = -1;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
            //if (sf::Keyboard::isKeyPressed(sf::Keyboard::F12)) player.movable = !player.movable;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                player.movable = true;
                player.Jump();

            }
        }

        for (int i = 0; i < 14; i ++){
                if (isColissioned(player.sprite, spikes[i].spike) && spikes[i].visible){
                    isDead = true;
                    continue;
                }
        }

        window.clear();
        window.draw(font);
        window.draw(midCircle);
        if (prevScore != score){
            if (score % 2 == 0 && score != 0){
                for (int i = 7; i < 14; i ++){
                    if (rand()%2 == 0){
                        spikes[i].visible = true;
                    }else{
                        spikes[i].visible = false;
                    }

                }

                for (int i = 0; i < 7; i ++){
                    spikes[i].visible = false;
                }
            }
            else if (score != 0){
                for (int i = 0; i < 7; i ++){
                    if (rand()%2 == 0){
                        spikes[i].visible = true;
                    }else{
                        spikes[i].visible = false;
                    }

                }

                for (int i = 7; i < 14; i ++){
                    spikes[i].visible = false;
                }

                }

            prevScore = score;
        }
        points.setString(std::to_string(score));
        points.setOrigin(points.getGlobalBounds().width/2.f, points.getGlobalBounds().height/2.f);
        window.draw(points);
        for (int i = 0; i < 14; i ++){
            if (spikes[i].visible){
                window.draw(spikes[i].spike);
            }

        }


        sf::Time elapsed = clock.restart();
        player.Move(elapsed);
        player.Draw(window);
        window.display();
    }

    return 0;
}
