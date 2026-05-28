export default function DefaultInput({
  type,
  name,
  placeholder,
  onChange,
  className,
}) {
  return (
    <input
      type={type}
      name={name}
      placeholder={placeholder}
      onChange={onChange}
      className={
        className
          ? className
          : " w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500 focus:ring-2 focus:ring-blue-500/30"
      }
    />
  );
}
