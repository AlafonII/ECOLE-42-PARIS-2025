import arrowUp from "../assets/images/page_assets/ARROW_UP.png";
import arrowDown from "../assets/images/page_assets/ARROW_DOWN.png";

type UsernameSlotOptions = {
	gapClass?: string;
	imageClass?: string;
	slotClass?: string;
};

const UsernameSlot = (
	index: number,
	ordinal: string,
	options: UsernameSlotOptions = {}
): string => {
	const gapClass = options.gapClass ?? "gap-2";
	const imageClass = options.imageClass ?? "h-9 w-9";
	const slotClasses = ["username-slot", "header-font"];
	if (options.slotClass) {
		slotClasses.push(options.slotClass);
	}

	return `
    <div class="flex flex-col items-center ${gapClass}">
      <button type="button" class="retro-btn username-up" data-index="${index}" aria-label="Increase ${ordinal} character"><img src="${arrowUp}" alt="Up" class="${imageClass}" /></button>
      <div class="${slotClasses.join(" ")}" data-index="${index}">A</div>
      <button type="button" class="retro-btn username-down" data-index="${index}" aria-label="Decrease ${ordinal} character"><img src="${arrowDown}" alt="Down" class="${imageClass}" /></button>
    </div>
  `;
};

export default UsernameSlot;
