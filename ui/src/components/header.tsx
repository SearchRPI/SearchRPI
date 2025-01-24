import ThemeSwitcher from "./theme-switcher";

const Header: React.FC = () => {
  return (
    <nav className="flex justify-between text-center">
      {/* TODO: Replace with actual logo as button */}
      <p>SearchRPI Logo</p>
      <div className="flex gap-5 ">
        <ThemeSwitcher />
      </div>
    </nav>
  );
};

export default Header;
