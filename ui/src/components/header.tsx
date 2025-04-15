import { Search } from "lucide-react";
import Image from "next/image";
import Link from "next/link";
import LightLogo from "./light-logo.png";
import DarkLogo from "./dark-logo.png";
import ThemeSwitcher from "./theme-switcher";
import { Button } from "./ui/button";
import { useEffect } from "react";
import { useRouter, useSearchParams } from "next/navigation";
import { useTheme } from "next-themes";
import {Input} from "@/components/ui/input";
import LogoSwitcher from "@/components/logo-switcher";

interface Props {
  searchQuery: string;
  setSearchQuery: React.Dispatch<React.SetStateAction<string | null>>;
  setShowResults: React.Dispatch<React.SetStateAction<boolean>>;
  setPageAndResults: () => void;
  onSubmit: () => void;
  minimal: boolean;
  handleKeyDown: (event: React.KeyboardEvent<HTMLInputElement>) => void
}

const Header: React.FC<Props> = ({ searchQuery, setSearchQuery, setShowResults, setPageAndResults, onSubmit , minimal, handleKeyDown }) => {
  const searchParams = useSearchParams();
  const router = useRouter();

  // Update logo based on theme
  const { theme } = useTheme();
  const logoSrc = theme === "dark" ? DarkLogo : LightLogo;

  // Remove the search params when clicking on the home page button
  const removeParam = () => {
    const params = new URLSearchParams(searchParams.toString());
    router.push(`${window.location.pathname}?${params.toString()}`);
    setSearchQuery("");
  };    

  return (
    <div>
      {!minimal ? (
        <nav className="flex justify-between items-center pr-5 backdrop-blur-xl">
          <Link href={"/"} className="font-bold text-2xl text-left px-0 w-[240px] h-[92px]" onClick={removeParam}>
            <LogoSwitcher width={"240px"} height={"92px"}/>
          </Link>
          
          <div className="flex justify-center items-center space-x-5">
            <Input
              value={searchQuery || ""}
              onChange={(e) => setSearchQuery(e.target.value)}
              onKeyDown={handleKeyDown}
              className="w-[40vw]"
              placeholder="Search here ..."
              id="SearchBox"
            />
            <Search className="cursor-pointer" onClick={onSubmit}/>
          </div>
          <ThemeSwitcher/>
        </nav>
      ):(
        <nav className="flex justify-end items-center h-[92px] pr-5">
          <ThemeSwitcher/>
        </nav>
      )}
    </div>


  );
};

export default Header;
