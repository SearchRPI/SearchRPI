import { GithubIcon } from "lucide-react";
import Link from "next/link";

const Footer: React.FC = () => {
  return (
    <nav className="flex justify-center items-center gap-x-10">
      <p>MIT License</p>
      <Link href="https://github.com/SearchRPI/Search/tree/main">
        <GithubIcon />
      </Link>
    </nav>
  );
};

export default Footer;
