import Link from "next/link";
import ThemeSwitcher from "./theme-switcher";
import { Button } from "./ui/button";

const Header: React.FC = () => {
  return (
    <nav className="flex justify-between p-5">
      {/* TODO: Replace with actual logo as button */}
      <Button
        variant={"ghost"}
        className="font-bold text-2xl text-left"
        asChild
      >
        <Link href={"/"}>SearchRPI Logo</Link>
      </Button>
      <ThemeSwitcher />
    </nav>
  );
};

export default Header;
