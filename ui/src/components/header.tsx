import { Search } from "lucide-react";
import Image from "next/image";
import Link from "next/link";
import LightLogo from "./light-logo.png";
import DarkLogo from "./dark-logo.png";
import ThemeSwitcher from "./theme-switcher";
import { Button } from "./ui/button";
import { useRouter, useSearchParams } from "next/navigation";
import { useTheme } from "next-themes";
import {Input} from "@/components/ui/input";

interface Props {
  searchQuery: string;
  setSearchQuery: React.Dispatch<React.SetStateAction<string | null>>;
  onSubmit: () => void;
  minimal: boolean;
}

const Header: React.FC<Props> = ({ searchQuery, setSearchQuery, onSubmit , minimal }) => {
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
        <nav className="flex justify-between items-center p-5 backdrop-blur-xl">
          {/* TODO: Replace with actual logo as button */}
          <Button
            variant={"ghost"}
            className="font-bold text-2xl text-left pt-8 px-0"
            asChild
            onClick={removeParam}
          >
            <Link href={"/"}>
              <Image src={logoSrc} alt="SearchRPI Logo" width={240} height={240}/>
            </Link>
          </Button>
          <div className="flex justify-center items-center space-x-5">
            {/* TODO: Handle "Submit" or "Enter" key press inside of the Input */}
            <Input
              value={searchQuery || ""}
              onChange={(e) => setSearchQuery(e.target.value)}
              className="w-[40vw]"
              placeholder="Search here ..."
              id="SearchBox"
            />
            <Search/>
          </div>
          <ThemeSwitcher/>
        </nav>
      ):(
        <nav className="flex justify-end items-center p-5">
          <ThemeSwitcher/>
        </nav>
      )}
    </div>
    // update header so that theme changer appears on landing page and entire header appears on search results page


  );
};

export default Header;
