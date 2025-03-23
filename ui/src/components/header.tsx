import Image from "next/image";
import Link from "next/link";
import LightLogo from "./light-logo.png";
import DarkLogo from "./dark-logo.png";
import ThemeSwitcher from "./theme-switcher";
import { Button } from "./ui/button";
import { useRouter, useSearchParams } from "next/navigation";
import { useTheme } from "next-themes";

interface Props {
  setSearchQuery: React.Dispatch<React.SetStateAction<string | null>>;
}

const Header: React.FC<Props> = ({ setSearchQuery }) => {
  const searchParams = useSearchParams();
  const router = useRouter();

  // Update logo based on theme
  const { theme } = useTheme();
  const logoSrc = theme === "dark" ? DarkLogo : LightLogo

  // Remove the search params when clicking on the home page button
  const removeParam = () => {
    const params = new URLSearchParams(searchParams.toString());
    router.push(`${window.location.pathname}?${params.toString()}`);
    setSearchQuery("");
  };

  return (
    <nav className="flex justify-between items-center p-5">
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
      <ThemeSwitcher />
    </nav>
  );
};

export default Header;
