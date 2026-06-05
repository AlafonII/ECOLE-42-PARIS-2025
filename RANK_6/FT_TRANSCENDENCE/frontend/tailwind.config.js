// Tailwind configuration file
module.exports = {
  // Tell Tailwind which files to scan for class names
  content: ["./index.html", "./src/**/*.{ts,tsx,js,jsx,html}"],

  // Customize or extend Tailwind’s default theme
  theme: {
    extend: {
      fontFamily: {
        title: ["title_font", "sans-serif"],
        body: ["body_font", "sans-serif"],
        header: ["header_font", "sans-serif"],
        sans: ["body_font", "ui-sans-serif", "system-ui", "sans-serif"],
      },
    },
  },

  // Extra Tailwind plugins
  plugins: [],
};
