import ThemeSwitcher from "./theme-switcher";

const Header: React.FC = () => {
  return (
    <nav className="flex justify-between text-center">
      <p>SearchRPI Logo</p>
      <div className="flex gap-5 ">
        <p>GitHub</p>
        <p>Dark/Light Mode</p>
        <ThemeSwitcher />
      </div>
    </nav>
  );
};

export default Header;
