import click

@click.group()
def main():
    pass


@main.command()
@click.option('--username', '-u', help="the username whose secret is desired", prompt="Enter your username")
@click.option('--password', '-p', help="the password of user", prompt=True, hide_input=True, confirmation_prompt=True)
def secret(username, password):
    click.echo(username)
    click.secho(password, fg='green')


if __name__ == '__main__':
    main()

