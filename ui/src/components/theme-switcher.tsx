"use client";

import { useState, useEffect } from "react";
import { useTheme } from "next-themes";
import { SunIcon, MoonIcon } from "lucide-react";
import { Button } from "./ui/button";

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
      console.log("setting to dark...");
    } else {
      setTheme("light");
      console.log("setting to light...");
    }
  };

  return (
    <Button key="theme-switcher" variant={"ghost"} onClick={handleTheme}>
      {theme === "dark" && <SunIcon size={18} />}
      {theme === "light" && <MoonIcon size={18} />}
    </Button>
  );
}
