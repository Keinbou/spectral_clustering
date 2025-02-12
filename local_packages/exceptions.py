class AbsentColumnException(Exception):
    def __init__(self, *args, **kwargs):
        super().__init__(*args)
        self.error_code = kwargs.get("error_code")