import Link from "next/link";
import ThemeSwitcher from "./theme-switcher";
import { Button } from "./ui/button";
import { useRouter, useSearchParams } from "next/navigation";

const Header: React.FC = () => {
  const searchParams = useSearchParams();
  const router = useRouter();

  // Remove the search params when clicking on the home page button
  const removeParam = () => {
    const params = new URLSearchParams(searchParams.toString());
    router.push(`${window.location.pathname}?${params.toString()}`);
  };

  return (
    <nav className="flex justify-between p-5">
      {/* TODO: Replace with actual logo as button */}
      <Button
        variant={"ghost"}
        className="font-bold text-2xl text-left"
        asChild
        onClick={removeParam}
      >
        <Link href={"/"}>SearchRPI Logo</Link>
      </Button>
      <ThemeSwitcher />
    </nav>
  );
};

export default Header;
