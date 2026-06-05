// Import all styles and page components
import "../styles/index.css";
import Router from "./router";
import Home from "./pages/Home";
import Login from "./pages/Login";
import Signup from "./pages/Signup";
import PongGame from "./pages/PongGame";
import Dashboard from "./pages/Dashboard";
import Friends from "./pages/Friends";
import LiveChat from "./pages/LiveChat";
import Profile from "./pages/Profile";
import Tournament from "./pages/Tournament";
import Settings from "./pages/Settings";
import Tournaments from "./pages/Tournaments";
import { getMe } from "./services/auth.service";

// Create the router and tell it where to render pages
const router = new Router("#app");

// Register each route with the component that should render that page
router.register("/", Home);
router.register("/login", Login);
router.register("/signup", Signup);
router.register("/pong", PongGame);
router.register("/pong/:id", PongGame);
router.register("/dashboard", Dashboard);
router.register("/friends", Friends);
router.register("/chat", LiveChat);
router.register("/profile", Profile);
router.register("/tournaments/:id", Tournament);
router.register("/tournaments", Tournaments);
router.register("/settings", Settings);


// Start the router and display the initial page
async function bootstrap() {
	try {
		const me = await getMe();

		if (me.success && me.data) {
			window.__ME__ = me.data;
		}
	} catch {
		window.__ME__ = null;
	}
	router.start();
}

bootstrap();
