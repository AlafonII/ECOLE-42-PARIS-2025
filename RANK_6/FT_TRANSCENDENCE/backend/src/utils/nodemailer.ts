import nodemailer from "nodemailer";

const transport = nodemailer.createTransport({
	host: process.env.MAIL_HOST,
	port: process.env.MAIL_PORT,
	secure: false,
	auth: {
		user: process.env.MAIL_USER,
		pass: process.env.MAIL_PASS,
	},
});

export async function emailOTP(to: string, otp: string) {
	await transport.sendMail({
		from: '"Security" <no-reply@ft_transcendence.fr>',
		to,
		subject: "Login verification code",
		html: `
			<p>Verification code :</p>
			<b>${otp}</b>
			<p>Validity: 5 minutes</p>
			`,
	});
}
