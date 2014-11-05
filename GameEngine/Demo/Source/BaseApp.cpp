#include "BaseApp.h"
#include "ILogger.h"
#include "ModifyMesh.h"

#include <sstream>
#include <iomanip>

const std::string BaseApp::m_GameTitle = "Demo 1.45";

BaseApp::BaseApp(void)
{
}

BaseApp::~BaseApp(void)
{
	SAFE_DELETE(m_Resourcemanager);
}

void BaseApp::init(void)
{
	m_MemUpdateDelay = 10.f;
	m_TimeToNextMemUpdate = 0.f;

	m_Window.init(getGameTitle(), getWindowSize());
	m_NewWindowSize = m_Window.getSize();

	//Created the resourceManager.
	m_Resourcemanager = new Res::ResourceManager(1024 * 15000);
	m_Resourcemanager->init();
	m_Resourcemanager->registerLoader(std::shared_ptr<Res::IResourceLoader>(new Res::OBJResourceLoader()));
	m_Resourcemanager->registerLoader(std::shared_ptr<Res::IResourceLoader>(new Res::MTLResourceLoader()));

	//Create the rendering component.
	bool fullscreen = false;
	m_Render.initialize(m_Window.getHandle(), m_Resourcemanager, (int)m_Window.getSize().x, (int)m_Window.getSize().y, fullscreen);
	ModifyMesh::initialize(&m_Render);

	m_Window.registerCallback(WM_CLOSE, std::bind(&BaseApp::handleWindowClose, this, std::placeholders::_1,
		std::placeholders::_2, std::placeholders::_3));
	m_Window.registerCallback(WM_EXITSIZEMOVE, std::bind(&BaseApp::handleWindowExitSizeMove, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	m_Window.registerCallback(WM_SIZE, std::bind(&BaseApp::handleWindowSize, this, std::placeholders::_1,
		std::placeholders::_2, std::placeholders::_3));
	
	//Setup user input mapping.
	InputTranslator::ptr translator(new InputTranslator);
	translator->init(&m_Window);

	translator->addKeyboardMapping(27, "Esc");
	translator->addKeyboardMapping(87, "moveForward");
	translator->addKeyboardMapping(83, "moveBackward");
	translator->addKeyboardMapping(65, "moveLeft");
	translator->addKeyboardMapping(68, "moveRight");
	translator->addKeyboardMapping(32, "moveUp");
	translator->addKeyboardMapping(VK_LCONTROL, "moveDown");
	
	translator->addMouseMapping(Axis::HORIZONTAL, false, "lookLeft");
	translator->addMouseMapping(Axis::HORIZONTAL, true, "lookRight");
	translator->addMouseMapping(Axis::VERTICAL, true, "lookUp");
	translator->addMouseMapping(Axis::VERTICAL, false, "lookDown");

	translator->addMouseButtonMapping(MouseButton::LEFT, "mouseLeftDown");
	translator->addMouseButtonMapping(MouseButton::LEFT, "mouseLeftRelease");

	m_InputQueue.init(std::move(translator));

	//Default camera variables.
	m_CameraDirection = Vector3(1, 0, 0);
	m_CameraPosition = Vector3(-30, 0, 0);
	m_CameraUp = Vector3(0,1,0);
	m_CameraSpeed = 0.5f;

	//Creating the level.
	m_Level.initialize(&m_Render, m_Resourcemanager, &m_CameraPosition);
}

void BaseApp::run(void)
{
	m_ShouldQuit = false;

	resetTimer();

	while (!m_ShouldQuit)
	{
		m_InputQueue.onFrame();
		m_Window.pollMessages();
		updateTimer();

		handleInput();


		//Camera movement.
		const InputState& state = m_InputQueue.getCurrentState();
		float forward = state.getValue("moveForward") - state.getValue("moveBackward");
		float right = state.getValue("moveRight") - state.getValue("moveLeft");
		float up = state.getValue("moveUp") - state.getValue("moveDown");
		
		if (up != 0.0f || right != 0.0f || forward != 0.0f)
		{
			using namespace DirectX;

			XMVECTOR dir = XMVectorSet(m_CameraDirection.x, 0, m_CameraDirection.z, 0);
			XMVECTOR cPos = XMLoadFloat3(&m_CameraPosition);
			XMVECTOR rightVec = XMVector3Cross(XMVectorSet(0, 1, 0, 0), dir);

			rightVec *= right;
			dir *= forward;

			dir = XMVector3Normalize(dir + rightVec);

			cPos = cPos + dir * m_CameraSpeed;
			XMStoreFloat3(&m_CameraPosition, cPos);
		}

		m_Level.update(m_DeltaTime);
		m_Render.updateCamera(m_CameraPosition, m_CameraDirection, Vector3(0, 1, 0));

		//Render.
		m_Render.begin();
		m_Level.draw();
		m_Render.draw();
		m_Render.end();

		updateDebugInfo();
	}
}

void BaseApp::shutdown(void)
{
	m_InputQueue.destroy();
}

bool BaseApp::handleWindowClose( WPARAM /*p_WParam*/, LPARAM /*p_LParam*/, LRESULT& p_Result )
{
	m_ShouldQuit = true;
	p_Result = 0;
	return true;
}

bool BaseApp::handleWindowExitSizeMove( WPARAM /*p_WParam*/, LPARAM p_LParam, LRESULT& p_Result )
{
	m_Window.setSize(m_NewWindowSize);				

	p_Result = 0;
	return true;
}

bool BaseApp::handleWindowSize( WPARAM p_WParam, LPARAM p_LParam, LRESULT& p_Result )
{
	m_NewWindowSize = Vector2(LOWORD(p_LParam),HIWORD(p_LParam));

	switch(p_WParam)
	{
	case SIZE_MAXIMIZED:
		{
			m_Window.setSize(m_NewWindowSize);
			m_Window.setIsMaximized(true);
			p_Result = 0;
			return true;
		}
	case SIZE_MAXHIDE:{return false;}
	case SIZE_MAXSHOW:{return false;}
	case SIZE_MINIMIZED:{return false;}
	case SIZE_RESTORED:
		{
			if(m_Window.getIsMaximized())
			{
				m_Window.setIsMaximized(false);
				m_Window.setSize(m_NewWindowSize);
				p_Result = 0;
				return true;
			}
			return false;
		}
	default:
		return false;
	}
}

void BaseApp::updateDebugInfo(void)
{
	m_TimeToNextMemUpdate -= m_DeltaTime;
	if (m_TimeToNextMemUpdate > 0.f)
	{
		return;
	}

	m_TimeToNextMemUpdate = m_MemUpdateDelay;

	std::string speed = "DeltaTime: " + std::to_string(m_DeltaTime) + " FPS: " + std::to_string(1.0f/m_DeltaTime);

	m_Window.setTitle(getGameTitle() + " | " + speed);
}

void BaseApp::resetTimer(void)
{
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	m_SecsPerCnt = 1.0f / (float)cntsPerSec;

	m_PrevTimeStamp = 0;
	m_CurrTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrTimeStamp);
	m_CurrTimeStamp--;
}

void BaseApp::updateTimer(void)
{
	m_PrevTimeStamp = m_CurrTimeStamp;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrTimeStamp);
	m_DeltaTime = ((m_CurrTimeStamp - m_PrevTimeStamp) * m_SecsPerCnt);// / 10.0f; // To debug the game at low refreshrate.
	static const float maxDeltaTime = 1.f / 24.f; // Up from 5.f. Animations start behaving wierd if frame rate drops below 24. 
	if (m_DeltaTime > maxDeltaTime)
	{
		m_DeltaTime = maxDeltaTime;
	}
}

void BaseApp::handleInput(void)
{
	const InputState& state = m_InputQueue.getCurrentState();

	float viewSensitivity = 0.005f;

	for (auto& in : m_InputQueue.getFrameInputs())
	{
		std::ostringstream msg;
		msg << "Received input action: " << in.m_Action << " (" << std::setprecision(2) << std::fixed << in.m_Value << ")";
		Logger::log(Logger::Level::TRACE, msg.str());

		if (in.m_Value > 0.5f && in.m_PrevValue <= 0.5f)
		{
			if (in.m_Action == "Esc")
			{
				m_ShouldQuit = true;
			}			
		}
		if (in.m_Action == "moveDown" && state.getValue("moveDown") > 0.5f)
		{
			m_CameraPosition.y -= m_CameraSpeed;
		}
		else if (in.m_Action == "moveUp" && state.getValue("moveUp") > 0.5f)
		{
			m_CameraPosition.y += m_CameraSpeed;
		}

		if (state.getValue("mouseLeftDown") > 0.5f)
		{
			m_InputQueue.lockMouse(true);
			if (in.m_Action == "lookRight")
			{
				movePlayerView(in.m_Value * viewSensitivity, 0.f);
			}
			else if (in.m_Action == "lookLeft")
			{
				movePlayerView(-in.m_Value * viewSensitivity, 0.f);
			}
			else if (in.m_Action == "lookUp")
			{
				movePlayerView(0.f, -in.m_Value * viewSensitivity);
			}
			else if (in.m_Action == "lookDown")
			{
				movePlayerView(0.f, in.m_Value * viewSensitivity);
			}
		}
		else 
			m_InputQueue.lockMouse(false);		
	}
}

std::string BaseApp::getGameTitle(void) const
{
	return m_GameTitle;
}

Vector2 BaseApp::getWindowSize(void) const
{
	// TODO: Read from user option

	const static Vector2 size = Vector2(1280, 720);
	return size;
}

void BaseApp::movePlayerView(float p_Yaw, float p_Pitch)
{
	using namespace DirectX;

	XMVECTOR vUp = XMVector3Normalize(XMLoadFloat3(&m_CameraUp));
	XMFLOAT3 forward = m_CameraDirection;
	XMVECTOR vForward = XMVector3Normalize(XMLoadFloat3(&forward));
	XMVECTOR vRight = XMVector3Cross(vUp, vForward);

	XMVECTOR rotationYaw = XMQuaternionRotationRollPitchYaw(0.f, p_Yaw, 0.f);
	XMVECTOR rotationPitch = XMQuaternionRotationAxis(vRight, p_Pitch);
	XMVECTOR rotation = XMQuaternionMultiply(rotationPitch, rotationYaw);
	XMVECTOR newUp = XMVector3Rotate(vUp, rotation);

	XMStoreFloat3(&forward, XMVector3Rotate(vForward, rotation));
	XMStoreFloat3(&m_CameraUp, newUp);

	if (forward.y > 0.9f || forward.y < -0.9f ||
		XMVectorGetX(XMVector3Dot(XMVectorSet(0.f, 1.f, 0.f, 0.f), newUp)) < 0.f)
	{
		return;
	}

	m_CameraDirection = forward;

	m_Render.updateCamera(m_CameraPosition, m_CameraDirection, m_CameraUp);
}