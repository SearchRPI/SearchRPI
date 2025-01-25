import { GithubIcon } from "lucide-react";
import Link from "next/link";

const Footer: React.FC = () => {
  return (
    <nav className="flex justify-center items-center gap-x-10">
      <p>A Spring 2025 RCOS Project</p>
      <Link href="https://github.com/SearchRPI/Search/tree/main">
        {/* FIX: Company icons are removed from Lucide-React, need to find a replacement as it is deprecated */}
        <GithubIcon />
      </Link>
    </nav>
  );
};

export default Footer;
