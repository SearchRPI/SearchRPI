"use client";

import { useState, useEffect } from "react";
import { useTheme } from "next-themes";
import Image from "next/image";
import LightLogo from "@/components/light-logo.png";
import DarkLogo from "@/components/dark-logo.png";

/**
 * Returns the logo.
 * 
 * Light mode will have black text.
 * Dark mode will have white text.
 */
export default function LogoSwitcher() {
  const [mounted, setMounted] = useState(false);
  const { theme, setTheme } = useTheme();

  // useEffect only runs on the client, so now we can safely show the UI
  useEffect(() => {
    setMounted(true);
    if (theme === "system") {
      const isSystemDark = window.matchMedia("(prefers-color-scheme: dark)").matches;
      isSystemDark ? setTheme("dark") : setTheme("light");
    }
  }, []);

  if (!mounted) {
    return null;
  }

  return (
    // biome-ignore lint/a11y/useKeyWithClickEvents lint/a11y/useFocusableInteractive lint/a11y/useSemanticElements: treat as a button, it should be fine
    <div>
      {theme === "dark" && 
        <Image
          src={DarkLogo}
          alt="SearchRPI Logo"
          style={{width: '600px', height: '300px', objectFit: 'cover'}}
        />}
      {theme === "light" && 
        <Image
          src={LightLogo}
          alt="SearchRPI Logo"
          style={{width: '600px', height: '300px', objectFit: 'cover'}}
        />}
    </div>
  );
}
