#include "DarknessEffect.h"
#include <stdexcept>

static const std::string DARKNESS_SHADER = R"(
    uniform sampler2D texture;
    uniform vec2 playerPos;
    uniform vec2 resolution;
    uniform float radius;
    uniform float softness;

    void main() {
        vec2 fragPos = gl_FragCoord.xy;
        vec2 pos = vec2(playerPos.x, resolution.y - playerPos.y);

        float dist = length(fragPos - pos);
        float light = 1.0 - smoothstep(radius - softness, radius + softness, dist);

        vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
        gl_FragColor = mix(vec4(0.0, 0.0, 0.0, 1.0), pixel, light);
    }
)";

DarknessEffect::DarknessEffect(sf::Vector2u windowSize, float radius, float softness)
    : m_windowSize(windowSize)
    , m_radius(radius)
    , m_softness(softness)
    , m_playerPos(0.f, 0.f)
{
    if (!sf::Shader::isAvailable())
        throw std::runtime_error("Shaders non supportés.");

    if (!m_shader.loadFromMemory(DARKNESS_SHADER, sf::Shader::Type::Fragment))
        throw std::runtime_error("Echec chargement shader.");
}

void DarknessEffect::setPlayerPos(sf::Vector2f pos) {
    m_playerPos = pos;
}

void DarknessEffect::setRadius(float radius) {
    m_radius = radius;
}

void DarknessEffect::setSoftness(float softness) {
    m_softness = softness;
}

void DarknessEffect::Render(sf::RenderWindow& window, const sf::RenderTexture& sceneTexture, sf::View cameraView) {
    m_windowSize = window.getSize();

    sf::Vector2i screenPosI = window.mapCoordsToPixel(m_playerPos, cameraView);
    sf::Vector2f screenPos = sf::Vector2f(
        static_cast<float>(screenPosI.x),
        static_cast<float>(screenPosI.y)
    );

    m_shader.setUniform("texture", sf::Shader::CurrentTexture);
    m_shader.setUniform("playerPos", screenPos);
    m_shader.setUniform("resolution", sf::Vector2f(m_windowSize));
    m_shader.setUniform("radius", m_radius);
    m_shader.setUniform("softness", m_softness);

    window.setView(window.getDefaultView());
    sf::Sprite scene(sceneTexture.getTexture());
    scene.setPosition({ 0.f, 0.f });
    window.draw(scene, &m_shader);
}