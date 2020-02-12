#include "MainMenuScene.h"
#include "Scenes.h"

#include <iostream>
#include <gf/UI.h>
#include <gf/Window.h>
#include <gf/Font.h>
#include <gf/Color.h>
#include <gf/Coordinates.h>
#include <gf/Widgets.h>
#include <gf/Unused.h>

namespace redsquare
{
    MainMenuScene::MainMenuScene(Scenes& scenes)
    : gf::Scene({1024, 576})
    , m_Scenes(scenes)
    , m_MenuChoice(MenuChoice::None)
    , m_Font(scenes.resources.getFont("font/arial.ttf"))
    , m_StartWidget("Start", m_Font)
    , m_QuitWidget("Quit", m_Font)
    {
        setClearColor(gf::Color::Black);

        m_StartWidget.setCallback([this]() { m_MenuChoice = MenuChoice::Start; });
        m_Container.addWidget(m_StartWidget);

        m_QuitWidget.setCallback([this]() { m_MenuChoice = MenuChoice::Quit; });
        m_Container.addWidget(m_QuitWidget);
    }

    void MainMenuScene::doHandleActions(gf::Window& window)
    {
        gf::unused(window);
    }

    void MainMenuScene::doProcessEvent(gf::Event &event)
    {
        switch (event.type) {
        case gf::EventType::MouseMoved:
            m_Container.pointTo(m_Scenes.getRenderer().mapPixelToCoords(event.mouseCursor.coords));
            break;

        case gf::EventType::MouseButtonPressed:
            m_Container.pointTo(m_Scenes.getRenderer().mapPixelToCoords(event.mouseButton.coords));
            m_Container.triggerAction();
            break;

        default:
            break;
        }
    }

    void MainMenuScene::doUpdate(gf::Time time)
    {
        switch (m_MenuChoice)
        {
            case MenuChoice::None:
                break;
            case MenuChoice::Start:
                m_Scenes.replaceScene(m_Scenes.connection, m_Scenes.glitchEffect, gf::seconds(0.8f));
                break;
            case MenuChoice::Quit:
                m_Scenes.popScene();
                break;
        }

        m_MenuChoice = MenuChoice::None;
    }

    void MainMenuScene::doRender(gf::RenderTarget& target, const gf::RenderStates& states)
    {
        gf::Coordinates coordinates(target);
        unsigned characterSize = coordinates.getRelativeCharacterSize(0.1f);

        auto startPosition = coordinates.getRelativePoint({ 0.5f, 0.2f });
        m_StartWidget.setCharacterSize(characterSize);
        m_StartWidget.setAnchor(gf::Anchor::Center);
        m_StartWidget.setPosition(startPosition);
        m_StartWidget.setDefaultTextColor(gf::Color::White);
        m_StartWidget.setDefaultTextOutlineColor(gf::Color::Black);
        m_StartWidget.setSelectedTextOutlineColor(gf::Color::Black);
        m_StartWidget.setTextOutlineThickness(characterSize * 0.05f);
        target.draw(m_StartWidget, states);

        auto quitPosition = coordinates.getRelativePoint({ 0.5f, 0.8f });
        m_QuitWidget.setCharacterSize(characterSize);
        m_QuitWidget.setAnchor(gf::Anchor::Center);
        m_QuitWidget.setPosition(quitPosition);
        m_QuitWidget.setDefaultTextColor(gf::Color::White);
        m_QuitWidget.setDefaultTextOutlineColor(gf::Color::Black);
        m_QuitWidget.setSelectedTextOutlineColor(gf::Color::Black);
        m_QuitWidget.setTextOutlineThickness(characterSize * 0.05f);
        target.draw(m_QuitWidget, states);
    }
}