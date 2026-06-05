import homeTitle from "../assets/images/page_titles/HOME_PAGE.png";
import { sessionService } from "../services/session.service";

const Home = (): string => {
	const isLoggedIn = sessionService.isLoggedIn();
	if (!isLoggedIn) {
		return `
    <section class="retro-card crt space-y-6 p-8">
      <div class="space-y-3">
        <div class="flex justify-center">
          <img src="${homeTitle}" alt="Welcome to ft_transcendence" class="mx-auto h-auto max-w-full" />
        </div>
      </div>
      <div class="flex flex-col items-center justify-center gap-3 sm:flex-row">
        <a data-link href="/login" class="retro-btn">LOGIN</a>
        <a data-link href="/pong" class="retro-btn">PLAY OFFLINE</a>
      </div>
    </section>
  `;
	}

	return `
    <section class="retro-card crt space-y-6 p-8">
      <div class="space-y-3">
        <div class="flex justify-center">
          <img src="${homeTitle}" alt="Welcome to ft_transcendence" class="mx-auto h-auto max-w-full" />
        </div>
      </div>
      <div class="flex justify-center">
        <a data-link href="/pong" class="retro-btn">Play Pong</a>
      </div>
    </section>
  `;
};

export default Home;
