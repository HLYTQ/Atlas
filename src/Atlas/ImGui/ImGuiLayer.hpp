#pragma once

#include "Atlas/Layer.hpp"

#include "Atlas/Events/ApplicationEvent.hpp"
#include "Atlas/Events/KeyEvent.hpp"
#include "Atlas/Events/MouseEvent.hpp"

namespace Atlas {

    class ImGuiLayer : public Layer {
    public:
	ImGuiLayer();
	~ImGuiLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Event& e) override;
	virtual void OnImGuiRender() override;

	void Begin();
	void End();

	void BlockEvents(bool block) { m_BlockEvents = block; }
	void SetDarkThemeColors();
    private:
	float m_Time = 0.0f;
	bool m_BlockEvents = true;
    };

} // namespace Atlas
