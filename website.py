from boto3.session import Session

ACCESS_KEY='AKIAXPP3X4VM6WXRRQV6'
SECRET_KEY='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'

session = Session(aws_access_key_id=ACCESS_KEY,
                  aws_secret_access_key=SECRET_KEY)
s3 = session.resource('s3')
your_bucket = s3.Bucket('jarvis2')

for s3_file in your_bucket.objects.all():
    if s3_file.key.endswith('jpg'):
        if not s3_file.key.startswith('images'):
            print('https://s3-us-west-2.amazonaws.com/jarvis2/'+s3_file.key)
