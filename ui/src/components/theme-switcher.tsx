"use client";

import { useState, useEffect } from "react";
import { useTheme } from "next-themes";
import { SunIcon, MoonIcon } from "lucide-react";

/**
 * The button that changes between the different themes, light or dark.
 *
 * Light mode will be plain white.
 * Dark mode will be plain black.
 */
export default function ThemeSwitcher() {
  const [mounted, setMounted] = useState(false);
  const { theme, setTheme } = useTheme();

  // useEffect only runs on the client, so now we can safely show the UI
  useEffect(() => {
    setMounted(true);
  }, []);

  if (!mounted) {
    return null;
  }

  const handleTheme = () => {
    if (theme === "light") {
      setTheme("dark");
    } else {
      setTheme("light");
    }
  };

  return (
    // biome-ignore lint/a11y/useKeyWithClickEvents lint/a11y/useFocusableInteractive lint/a11y/useSemanticElements: treat as a button, it should be fine
    <div role="button" onClick={handleTheme} >
      {theme === "dark" && <SunIcon size={28} />}
      {theme === "light" && <MoonIcon size={28} />}
    </div>
  );
}
